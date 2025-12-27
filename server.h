#
/*
 *    Copyright (C)  2025
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of sdrplay_tcp
 *
 *    sdrplay_tcp is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    ssdrplay_tcp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with sdrplay_tcp; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#pragma once

#include	<QObject>
#include	<QWidget>
#include	<QByteArray>
#include	<QCloseEvent>
#include	<QTimer>
#include	<atomic>
#include	<complex>
#include	"ui_server.h"
#include	"ringbuffer.h"
#include	"device-handler.h"
#include	"errorlog.h"
#include        "settings-handler.h"

#include	"spectrum-scope.h"
#include	"fft.h"

class	QSettings;
class	TcpHandler;
/*
 *	The main gui object. It inherits from
 *	QDialog and the generated form
 */
class Server: public QWidget, private Ui_sdrplayServer {
Q_OBJECT
public:
		Server	(QSettings *, QWidget *parent = NULL);
		~Server	();

private:
	errorLogger	theErrorLogger;
	QSettings	*serverSettings;
	common_fft	fftHandler;
	TcpHandler	*handler_1234;
	RingBuffer<std::complex<uint8_t>> _I_Buffer;
	deviceHandler	*theDevice;
	int		portNumber;
	spectrumScope	*theScope;
public slots:
	void		dispatch		(QByteArray &);
	void		newData			(int);
	void		handle_portSelector	(int);
	void		setStatus		(const QString &);
};

