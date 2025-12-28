


#pragma once

#include	<stdint.h>
#include	<complex>
#include	<vector>
#include	"base-converter.h"

//#include	"fir-filter.h"

class	interpolator:public baseConverter {
public:
		interpolator	(int, int);
		~interpolator	();
	bool	process		(std::complex<int16_t>,
	                                   std::complex<int16_t> &);
private:
	int			inrate;
	int			outrate;
	std::vector<std::complex<int16_t>> outVector;
	int			outCounter;
//	firFilter	theFilter;
	std::vector<std::complex<int16_t>> convBuffer;
        int                     convBufferSize;
        int                     convIndex;
        std::vector<int16_t>	mapTable_int;
        std::vector<float>	mapTable_float;
};

