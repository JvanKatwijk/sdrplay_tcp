#
/*
 *    Copyright (C) 2025
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the sdrplay_tcp
 *
 *    sdrplay_tcp is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    sdrplay_tcp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with sdrplay_tcp; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#pragma once

#include	<cstdint>
#include	<QFrame>
#include	<QThread>
#include	"ringbuffer.h"
//	We provide a simple interface to the devices. Note that
//	it is not just an abstract interface,
//	it provides a number of shared functions (like hide and show).
//	and - since it provides default interfaces for all functions -
//	there is no need for e.g. a file handling interface
//	to implement functions for gain or frequency
class	deviceHandler: public QThread {
Q_OBJECT
public:
			deviceHandler	(RingBuffer<std::complex<uint8_t>> *);
virtual			~deviceHandler	();
virtual		bool	restartReader	(int32_t freq);
virtual		void	stopReader	();
virtual		void	resetBuffer	();
virtual		int16_t	bitDepth	() { return 10;}
virtual		QString deviceName	();
virtual		int32_t	getVFOFrequency	();
virtual		int32_t	getRate		();
//
//	implementing the "commands" (to start with)
virtual		void	tcp_setFrequency	(int);
virtual		void	tcp_setSampleRate	(int);
virtual		void	tcp_setGainMode		(bool);
virtual		void	tcp_setGain		(int);
virtual		void	tcp_setAgc		(int);
virtual		void	tcp_setPpm		(int);
virtual		void	tcp_setBiasT		(bool);

protected:
		QFrame	myFrame;
		int32_t	lastFrequency;
	        int	theGain;
	        RingBuffer<std::complex<uint8_t>> *_I_Buffer;
};

