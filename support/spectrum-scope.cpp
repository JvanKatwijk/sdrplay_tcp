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
 *    sdrplay_tcp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with sdrplay_tcp; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include	"spectrum-scope.h"
#include	<QSettings>
#include        <QColor>
#include        <QPen>

static inline
int	valueFor (int bd) {
int result	= 1;
	while (bd > 1) {
	   result <<= 1;
	   bd --;
	}
	return result;
}


	spectrumScope::spectrumScope (QwtPlot *serverScope,
	                              int displaySize,
	                              QSettings	*serverSettings):
	                                  spectrumCurve ("") {
QString	colorString	= "black";
bool	brush;

	this		-> serverSettings	= serverSettings;
	this		-> displaySize		= displaySize;
	colorString	= "white";
	displayColor	= QColor (colorString);
	colorString	= "black";
	gridColor	= QColor (colorString);
	colorString	= "cyan";
	curveColor	= QColor (colorString);
	plotgrid	= serverScope;
	plotgrid	-> setCanvasBackground (displayColor);
	grid		= new QwtPlotGrid;
	grid		-> setMajorPen (QPen(gridColor, 0, Qt::DotLine));
	grid		-> enableXMin (true);
	grid		-> enableYMin (true);
	grid		-> setMinorPen (QPen(gridColor, 0, Qt::DotLine));
	grid		-> attach (plotgrid);
	spectrumCurve. setPen (QPen (curveColor, 2.0));
	spectrumCurve. setOrientation (Qt::Horizontal);
	spectrumCurve. setBaseline	(get_db (0));
	spectrumCurve. attach (plotgrid);
	plotgrid	-> enableAxis (QwtPlot::yLeft);
	bitDepth	= 8;
	normalizer	= 256.0;
}

	spectrumScope::~spectrumScope	() {
	delete		grid;
}

void	spectrumScope::display		(double *X_axis,
	                                 double *Y_value, int Amp) {
	float Max	= Amp / 50.0 * (-get_db (0));
	plotgrid	-> setAxisScale (QwtPlot::xBottom,
				         (double)X_axis [0],
				         X_axis [displaySize - 1]);
	plotgrid	-> enableAxis (QwtPlot::xBottom);
	plotgrid	-> setAxisScale (QwtPlot::yLeft,
				         get_db (0), get_db (0) + 2.5 * Max);

	spectrumCurve. setBaseline (get_db (0));
	Y_value [0]		= get_db (0);
	Y_value [displaySize - 1] = get_db (0);

	spectrumCurve. setSamples (X_axis, Y_value, displaySize);
	plotgrid	-> replot (); 
}

float   spectrumScope::get_db (float x) {
        return 20 * log10 ((x + 1) / (float)(normalizer));
}

