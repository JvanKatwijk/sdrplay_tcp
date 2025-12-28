#
/*
 *    Copyright (C) 2025
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of sdrplay_tcp
 *
 *    sdrplay_tcp is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation version 2 of the License.
 *
 *    sdrplay_tcp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with sdrplay_tcp if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

//
//	This file will be included in the sdrplay-controller.cpp file
//
//	The file contains the definitions of the interface data between
//	the calling and the callee thread
#pragma once

#define RESTART_REQUEST         0100
#define STOP_REQUEST            0101
#define LNA_REQUEST             0102
#define ANTENNASELECT_REQUEST   0103
#define	NOTCH_REQUEST		0120
#define	TUNERSELECT_REQUEST	0121

#define SETFREQUENCY_REQUEST	0104
#define	SAMPLERATE_REQUEST	0105
#define	BANDWIDTH_REQUEST	0106
#define AGC_REQUEST             0107
#define GRDB_REQUEST            0110
#define PPM_REQUEST             0111
#define	BIAS_T_REQUEST		0112
#include	<QSemaphore>
#include	<stdio.h>

class generalCommand {
public:
	int	cmd;
	bool	result;
	QSemaphore waiter;
	generalCommand (int command):
	                waiter (0){
	   this -> cmd = command;
	}
	generalCommand () {}
};

class	restartRequest: public generalCommand {
public:
	int	freq;
	bool	result;
	restartRequest (int newFrequency):
	   generalCommand (RESTART_REQUEST) {
	   this	-> freq = newFrequency;
	}

	~restartRequest	() {}
};

class	stopRequest: public generalCommand {
public:
	stopRequest ():
	   generalCommand (STOP_REQUEST) {
	}

	~stopRequest	() {}
};

class	lnaRequest: public generalCommand {
public:
	int	lnaState;
	lnaRequest (int lnaState):
	           generalCommand (LNA_REQUEST) {
	   this	-> lnaState	= lnaState;
	}
	~lnaRequest	() {}
};

class	antennaRequest: public generalCommand {
public:
	int	antenna;
	antennaRequest (int antenna) :
	           generalCommand (ANTENNASELECT_REQUEST) {
	   this	-> antenna	= antenna;
	}
	~antennaRequest	() {}
};

class	notch_Request: public generalCommand {
public:
	bool	checked;
	notch_Request (bool notch_value):
	            generalCommand (NOTCH_REQUEST) {
	   this -> checked = notch_value;
	}
	~notch_Request	() {}
};

class	tunerRequest: public generalCommand {
public:
	int	tuner;
	tunerRequest (int tuner) :
	           generalCommand (TUNERSELECT_REQUEST) {
	   this	-> tuner	= tuner;
	fprintf (stderr, "Creating a message for set_tuner for tuner %d\n", tuner);
	}
	~tunerRequest	() {}
};

class set_frequencyRequest: public generalCommand {
public:
	int newFreq;
	set_frequencyRequest (int newFreq):
	   generalCommand (SETFREQUENCY_REQUEST) {
	   this	-> newFreq = newFreq;
	}

	~set_frequencyRequest	() {}
};

class set_samplerateRequest: public generalCommand {
public:
	int samplerate;
	set_samplerateRequest (int samplerate):
	   generalCommand (SAMPLERATE_REQUEST) {
	   this	-> samplerate = samplerate;
	}

	~set_samplerateRequest	() {}
};

class set_bandwidthRequest: public generalCommand {
public:
	int bandwidth;
	set_bandwidthRequest (int bandwidth):
	   generalCommand (BANDWIDTH_REQUEST) {
	   this	-> bandwidth = bandwidth;
	}

	~set_bandwidthRequest	() {}
};

class agcRequest: public generalCommand {
public:
	int	setPoint;
	bool	agcMode;
	agcRequest (bool onoff, int setPoint) :
	                generalCommand (AGC_REQUEST) {
	   this	-> setPoint	= setPoint;
	   this	-> agcMode	= onoff;
	}
	~agcRequest	() {}
};

class	GRdBRequest: public generalCommand {
public:
	int	GRdBValue;
	GRdBRequest (int GRdBValue):
	               generalCommand (GRDB_REQUEST) {
	   this	-> GRdBValue	= GRdBValue;
	}
	~GRdBRequest	() {}
};

class	ppmRequest: public generalCommand {
public:
	int	ppmValue;
	ppmRequest (int ppmValue):
	           generalCommand (PPM_REQUEST) {
	   this	-> ppmValue	= ppmValue;
	}
	~ppmRequest	() {}
};

class	biasT_Request: public generalCommand {
public:
	bool	checked;
	biasT_Request (bool biasT_value):
	            generalCommand (BIAS_T_REQUEST) {
	   this -> checked = biasT_value;
	}
	~biasT_Request	() {}
};
	
