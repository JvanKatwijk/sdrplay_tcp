#
/*
 *    Copyright (C)  2025
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the sdrplayServer
 *
 *    sdrplayServerr is free software; you can redistribute it and/or modify
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
 *    along with sdrplayServerr; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#pragma once

#include	<QObject>
#include	<QByteArray>
#include	<complex>
#include	<atomic>
#include        <QTcpSocket>
#include        <QTcpServer>
#include        <QAbstractSocket>

class	Server;
/*
 *	the actual server
 */
class TcpHandler: public QTcpServer {
Q_OBJECT
public:
		TcpHandler	(Server *, int portNumber);
		~TcpHandler	();
	void	newData		(std::complex<uint8_t> *, int);
private:
//	QSettings	*spectrumSettings;
	Server		*theServer;
	std::atomic<bool> running;
	QTcpSocket	*theSocket;
public slots:
	void		newConnection	();
	void		readSocket	();
	void		discardSocket	();
	void		onSocketError	(QAbstractSocket::SocketError);
signals:
	void		dispatch	(QByteArray &);
	void		setStatus	(const QString &);
};

