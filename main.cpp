#
/*
 *    Copyright (C) 2025
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the sdrplay-server
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
 *
 *	Main program
 */
#include	<Qt>
#include	<QApplication>
#include	<QSettings>
#include	<QDir>
#include	"server.h"

#define	initFileName	"/home/jan/.server"
int	main (int argc, char **argv) {
/*
 *	The default values
 */
QSettings	*ISettings;		/* input .ini file	*/
Server	*myServer;

	ISettings	= new QSettings (initFileName, QSettings::IniFormat);
/*
 *	Before we connect control to the gui, we have to
 *	instantiate
 */
	QApplication a (argc, argv);
	QFile file (":res/globstyle.qss");
	if (file .open (QFile::ReadOnly | QFile::Text)) {
	   a. setStyleSheet (file.readAll ());
	   file.close ();
	}

	myServer = new Server (ISettings);

	myServer -> show ();
	a. exec ();
/*
 *	done:
 */
	fflush (stdout);
	fflush (stderr);
	qDebug ("It is done\n");
	ISettings	-> sync ();
	delete	myServer;
	exit (1);
}

