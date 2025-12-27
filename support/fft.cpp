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
 *
 */
#include "fft.h"
#include	<cstring>
/*
 */
	common_fft::common_fft (int32_t fft_size) {
int32_t	i;

	if ((fft_size & (fft_size - 1)) == 0)
	   this	-> fft_size = fft_size;
	else
	   this -> fft_size = 4096;	/* just a default	*/

	vector	= (std::complex<float> *)
	             FFTW_MALLOC (sizeof (std::complex<float>) * fft_size);
	plan	= FFTW_PLAN_DFT_1D (fft_size,
	                            reinterpret_cast <fftwf_complex *>(vector),
	                            reinterpret_cast <fftwf_complex *>(vector),
	                            FFTW_FORWARD, FFTW_ESTIMATE);
}

	common_fft::~common_fft () {
	   FFTW_DESTROY_PLAN (plan);
	   FFTW_FREE (vector);
}

void	common_fft::do_FFT (std::complex<float> *v, int size) {
	if (size >  fft_size)
	   size = fft_size;
	for (int i = 0; i < size; i ++)
	   vector [i] = v [i];
	FFTW_EXECUTE (plan);
	for (int i = 0; i < size; i ++)
	   v [i] = vector [i];
}
