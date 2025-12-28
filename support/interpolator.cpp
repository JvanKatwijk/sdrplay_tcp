

#include	"interpolator.h"

	interpolator::interpolator (int inrate, int outrate):
//	                                       theFilter (outrate / 2, 15, inrate),
	                                       convBufferSize (inrate / 1000 + 1),
	                                       convBuffer (inrate / 1000),
	                                       mapTable_int (outrate / 1000),
	                                       mapTable_float (outrate / 1000) {
	convIndex	= 0;
	this	-> inrate	= inrate;
	this	-> outrate	= outrate;
	this	-> outVector. resize (outrate / 1000);
	outCounter	= -1;
}

	interpolator::~interpolator	() {}

static
std::complex<int16_t> cmul (std::complex<int16_t> x, float y) {
	return std::complex<int16_t> (real (x) * y,
	                              imag (x) * y);
}

bool	interpolator::process	(std::complex<int16_t> inVal,
	                              std::complex<int16_t> &outVal) {
	convBuffer [convIndex ++] = inVal;
//	convBuffer [convIndex ++] = theFilter -> Pass (inVal);
	if ((outCounter < 0) || (outCounter >= outrate / 1000))
	   return false;
	outVal	= outVector [outCounter ++];

	if (convIndex <= inrate / 1000) 
	   return true;

	for (int i = 0; i < outrate; i ++) {
	   int16_t inpBase     = mapTable_int [i];
	   float   inpRatio    = mapTable_float [i];
	   outVector [i]        = cmul (convBuffer [inpBase + 1], inpRatio) +  
                                  cmul (convBuffer [inpBase], (1 - inpRatio));
	}
	convBuffer [0] = convBuffer [convBufferSize - 1];
	convIndex = 1;
	outCounter = 0;
	return true;
}

