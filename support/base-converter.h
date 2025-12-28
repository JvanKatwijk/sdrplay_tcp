

#pragma once

#include	<stdint.h>
#include	<complex>

class	baseConverter {
public:
		baseConverter	();
		~baseConverter	();
virtual	bool	process		(std::complex<int16_t>,
	                                   std::complex<int16_t> &);
};

