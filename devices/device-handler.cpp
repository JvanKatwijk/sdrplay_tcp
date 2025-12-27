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

#include	"device-handler.h"
//
	deviceHandler::deviceHandler	(RingBuffer<std::complex<uint8_t>> *b):
	                        myFrame (nullptr) {
	_I_Buffer	= b;
	lastFrequency	= 100000;
	theGain		= 50;
}

	deviceHandler::~deviceHandler	() {
}

bool	deviceHandler::restartReader	(int32_t freq) {
	lastFrequency	= freq;
	return true;
}

void	deviceHandler::stopReader	() {
}

void	deviceHandler::resetBuffer	() {
}

QString	deviceHandler::deviceName	() {
	return "";
}

int	deviceHandler::getVFOFrequency	() {
	return lastFrequency;
}

int	deviceHandler::getRate		() {
	return 2048000;
}
//
//
void	deviceHandler::tcp_setFrequency		(int freq) {
	(void)freq;
}

void	deviceHandler::tcp_setSampleRate	(int rate) {
	(void)rate;
}

void	deviceHandler::tcp_setGainMode		(bool gainMode) {
	(void)gainMode;
}

void	deviceHandler::tcp_setGain		(int gain) {
	(void)gain;
}

void	deviceHandler::tcp_setAgc		(int gain) {
	(void)gain;
}

void	deviceHandler::tcp_setPpm		(int ppm) {
	(void)ppm;
}

void	deviceHandler::tcp_setBiasT		(bool b) {
	(void)b;
}

