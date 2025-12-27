#
/*
 *    Copyright (C)  2025
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the sdrplayServer
 *
 *    sdrplay_tcp is free software; you can redistribute it and/or modify
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


#include	"tcpHandler.h"
#include	"server.h"
/*
 *	the actual server
 */
	TcpHandler::TcpHandler	(Server *theServer, int portNumber) {
	this	-> theServer	= theServer;
	if (!this -> listen (QHostAddress::Any, portNumber)) 
	   throw (11);
	setStatus ("I am listening");
	connect (this, &QTcpServer::newConnection,
	         this, &TcpHandler::newConnection);
	connect (this, &TcpHandler::dispatch,
	         theServer, &Server::dispatch);
	connect (this, &TcpHandler::setStatus,
	         theServer, &Server::setStatus);
	running. store (true);
}

	TcpHandler::~TcpHandler	() {
}

static bool	connected	= false;
void    TcpHandler::newConnection      () {
	if (this -> hasPendingConnections ()) {
	   theSocket = this -> nextPendingConnection ();
	   connect (theSocket, &QTcpSocket::readyRead,
	            this, &TcpHandler::readSocket);
	   connect (theSocket, &QAbstractSocket::disconnected,
                    this, &TcpHandler::discardSocket);
           connect  (theSocket, &QTcpSocket::errorOccurred,
                     this, &TcpHandler::onSocketError);
	   connected = true;
	   setStatus ("I am connected");
        }
}

void	TcpHandler::discardSocket () {
QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
	socket -> deleteLater ();
	socket	-> close ();
}

void	TcpHandler::onSocketError (QAbstractSocket::SocketError socketerror) {
QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
	fprintf (stderr, "Error %d\n", (int)socketerror);
	theSocket	-> close ();
	theSocket	= nullptr;
	setStatus ("I am listening");
}

void    TcpHandler::readSocket () {
	if (theSocket == nullptr)
	   return;
	QByteArray data	= theSocket -> readAll ();
	dispatch (data);
}

void	TcpHandler::newData	(std::complex<uint8_t> *v, int size) {
QByteArray theData;
	if (!connected || (theSocket == nullptr))
	   return;
	theData. resize (2 * size);
	for (int i = 0; i < size; i ++) {
	  theData [2 * i]	= real (v [i]);
	  theData [2 * i + 1]	= imag (v [i]);
	}
	theSocket	-> write (theData);
}

