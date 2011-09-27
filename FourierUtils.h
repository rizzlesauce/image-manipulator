/*
 * FourierUtils.h
 *
 *  Created on: Mar 11, 2010
 *      Author: ross
 */

#ifndef FOURIERUTILS_H_
#define FOURIERUTILS_H_
#include <fftw3.h>

#define FFTW_INDEX2D(x, y, height) ((int)y + (int)height * (int)x)
#define FFTW_INDEX2D_X(i, height) ((int)i / (int)height)
#define FFTW_INDEX2D_Y(i, height) ((int)i % (int)height)

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
    static void complexTimesComplex(fftw_complex& first, fftw_complex& second, fftw_complex& result);
    static void copyComplex(fftw_complex& original, fftw_complex& copy);
    static void complexToRealImaginary(fftw_complex& complex, double& real, double& imaginary);
};

#endif /* FOURIERUTILS_H_ */
