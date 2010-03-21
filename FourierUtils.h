/*
 * FourierUtils.h
 *
 *  Created on: Mar 11, 2010
 *      Author: ross
 */

#ifndef FOURIERUTILS_H_
#define FOURIERUTILS_H_
#include <fftw3.h>

#define FFTW_INDEX2D(x, y, height) (y + height * x)

class FourierUtils {
public:
	FourierUtils();
	virtual ~FourierUtils();

    static void complexToPower(fftw_complex&, double&);
	static void complexToMagnitude(fftw_complex&, double&);
	static void complexToPhase(fftw_complex&, double&);
	static void complexToReal(fftw_complex&, double&);
	static void complexToImaginary(fftw_complex&, double&);
	static void realTimesComplex(double&, fftw_complex&, fftw_complex&);
	static void realToComplex(double&, fftw_complex&);
	static void justRealToComplex(double&, fftw_complex&);
	static void imaginaryToComplex(double&, fftw_complex&);
	static void realImaginaryToComplex(double&, double&, fftw_complex&);
	static void normalize1DSignal(fftw_complex*, fftw_complex*, int);

};

#endif /* FOURIERUTILS_H_ */
