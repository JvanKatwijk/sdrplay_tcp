#
/*
 *    Copyright (C) 2025
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the sdrplayServer
 *
 *    sdrplayServer is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    sdrplayServer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with sdrplayServer; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include	"server.h"
#include	<QSettings>
#include	<QMessageBox>
#include	"tcpHandler.h"
#include	"sdrplay-handler-v3.h"
#include	"fft.h"
#ifdef __MINGW32__
#include	<iostream>
#endif
#

#define	DISPLAYSIZE	512
static float mapTable [256];
	Server::Server (QSettings	*Si,
	                QWidget		*parent):
	                    QWidget (parent),
	                    _I_Buffer (32 * 32768),
	                    theErrorLogger (Si),
	                    fftHandler (DISPLAYSIZE) {
// 	the setup for the generated part of the ui
	setupUi (this);
	serverSettings		= Si;

	for (int i = 0; i < 256; i ++)
	   mapTable [i] = (float) (i - 128.0) / 128.0;

	portNumber	= value_i (serverSettings, "TCP_SETTINGS",
	                                          "portNumber", 1234);
	portSelector	-> setValue (portNumber);
//	handler_1234 reads commands and transmits 8 bit data
	try {
	   handler_1234	= new TcpHandler (this, portNumber);
	} catch (...) {
	   statusLabel	-> setText ("Could not allocate handler");
	   return;
	}
//	handler_1235 just reads commands
//	handler_1235	= new handler (this, 1235);
//	connect (handler_1235, &hander::dispatch,
//	         this, &Server::dispatch);

	try {
	   theDevice	= new sdrplayHandler_v3 (this, Si,
	                                          &_I_Buffer, &theErrorLogger);
	} catch (...) {
	   fprintf (stderr, "no device\n");
	   return;
	}
	theScope	=  new spectrumScope (spectrumDisplay,
	                                           DISPLAYSIZE, Si);
	setStatus ("I am listening");
	theDevice	-> restartReader (2200000);
}

	Server::~Server () {
	delete theScope;
	delete theDevice;
}

uint32_t fetch (QByteArray &b, int nrBytes, int &index) {
uint32_t result = 0;
	for (int i = 0; i < nrBytes; i ++) {
	   result <<= 8;
	   result |=  (uint8_t)(b [index ++]);
	}
	return result;
}

void	Server::setStatus	(const QString &text) {
	statusLabel	-> setText (text);
}

void	Server::dispatch (QByteArray &data) {
int	index = 0;
	while (index < data. size ()) {
	   uint8_t command = data [index ++];
	   switch (command) {
	      case 0x1: {		// set new frequency
	         uint32_t frequency	= fetch (data, 4, index);
	         theDevice	-> tcp_setFrequency (frequency);
	         freqLabel	-> setText (QString::number (frequency));
	         commandLabel	-> setText ("set frequency");
	         break;
	      }
	      case 0x2: {		// fetch samplerate
	         uint32_t samplerate = fetch (data, 4, index);
	         theDevice	-> tcp_setSampleRate (samplerate);
	         rateLabel	-> setText (QString::number (samplerate));
	         commandLabel	-> setText ("set samplerate");
	         break;
	      }
	      case 0x3:		// tuner gain Mode
	         fprintf (stderr, "gain mode %d\n", data [index + 3] != 1);
//	         theDevice	-> tcp_setGainMode (data [index + 3] != 1);
	         commandLabel	-> setText ("set tuner gain Mode");
	         index += 4;
	         break;
	      case 0x4: {		// tuner gain, in tenths of a dB
	         uint32_t gain = fetch (data, 4, index);
	         theDevice	-> tcp_setGain	(gain);
	         commandLabel	-> setText ("set tuner gain");
	         break;
	      }
	      case 0x5: {		// frequency correction in ppM
	         uint32_t ppM = fetch (data, 4, index);
	         theDevice	-> tcp_setPpm (ppM);
	         commandLabel	-> setText ("set PPM");
	         break;
	      }
	      case 0x6: {		// if gain Level
	         uint16_t stage	= fetch (data, 2, index);
	         uint16_t gain	= fetch (data, 2, index);
	         commandLabel	-> setText ("set gainLevel");
	         break;
	      }
	      case 0x7: {		// put the tuner in testmode
	         uint32_t testMode = fetch (data, 4, index);
	         commandLabel	-> setText ("set testmode");
	         break;
	      }
	      case 0x8: {		// set the automatic gain correction
	         uint32_t agc	= fetch (data, 4, index);
	         theDevice	-> tcp_setAgc (agc);
	         commandLabel	-> setText ("set agc");
	         break;
	      }
	      case 0x9: {		// set direct sampling
	         uint32_t directSampling = fetch (data, 4, index);
	         commandLabel	-> setText ("set direct sampling");
	         break;
	      }
	      case 0xa: {		// enable offset tuning
	         uint32_t eot	= fetch (data, 4, index);
	         commandLabel	-> setText ("set enable offset tuning");
	         break;
	      }
	      case 0xd: {		// tuner gain by index
	         uint32_t gainIndex = fetch (data, 4, index);
	         commandLabel	-> setText ("set tuner gain by index");
	         break;
	      }
	      case 0xe: {		// set bias Tee
	         uint32_t biasTee = fetch (data, 4, index);
	         theDevice	-> tcp_setBiasT (biasTee != 0);
	         commandLabel	-> setText ("set biasT");
	         break;
	      }
	      default:
	         index += 4;
	         break;
	   }
	}
}

void	Server::newData	(int n) {
static int counter	= 0;
	(void)n;
	int	freq	= theDevice	-> getVFOFrequency ();
	int	rate	= theDevice	-> getRate ();
	double	X_axis [DISPLAYSIZE];
	double	Y_values [DISPLAYSIZE];
	std::complex<float> V	[DISPLAYSIZE];
	for (int i = 0; i < DISPLAYSIZE; i ++) 
	   X_axis [i] = (freq - rate / 2 + i * rate / (float)DISPLAYSIZE) / 1000;
	while (_I_Buffer. GetRingBufferReadAvailable () > 2048) {
	   std::complex<uint8_t> buffer [2048];
	   _I_Buffer. getDataFromBuffer (buffer, 2048);
	   counter ++;
	   if (counter > 200) {
	      for (int i = 0; i < DISPLAYSIZE; i ++)
	         V [i] = std::complex<float> (mapTable [real (buffer [i])],
	                                      mapTable [imag (buffer [i])]);
	      fftHandler. do_FFT (V, DISPLAYSIZE);
	      for (int i = 0; i < DISPLAYSIZE; i ++)
	         Y_values [i] =  abs (V [(i + DISPLAYSIZE / 2) % DISPLAYSIZE]);
	      theScope -> display (X_axis, Y_values, spectrumAmplitude -> value ());
	      counter = 0;
	   }
	   handler_1234 -> newData (buffer, 2048);
	}
}

void	Server::handle_portSelector	(int n) {
	portNumber	= n;
}

