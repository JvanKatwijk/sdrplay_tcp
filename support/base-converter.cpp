


#include	"base-converter.h"

	baseConverter::baseConverter	() {}
	baseConverter::~baseConverter	() {}
bool	baseConverter::process		(std::complex<int16_t> inVal,
	                                      std::complex<int16_t> & outVal) {
	outVal = inVal;
	return true;
}

