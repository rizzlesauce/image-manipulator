/*
 * FourierUtils.cpp
 *
 *  Created on: Mar 11, 2010
 *      Author: ross
 */

#include "FourierUtils.h"
#include <fftw3.h>
#include <cmath>

FourierUtils::FourierUtils() {
	// TODO Auto-generated constructor stub

}

FourierUtils::~FourierUtils() {
	// TODO Auto-generated destructor stub
}

void FourierUtils::complexToMagnitude(fftw_complex& complex, double& magnitude) {
	double power;
	complexToPower(complex, power);
	magnitude = sqrt(power);
}

void FourierUtils::complexToPhase(fftw_complex& complex, double& phase) {
	double real, imaginary;
	complexToReal(complex, real);
	complexToImaginary(complex, imaginary);

	phase = atan2(imaginary, real);
}

void FourierUtils::complexToPower(fftw_complex& complex, double& power) {
	double real, imaginary;
	complexToReal(complex, real);
	complexToImaginary(complex, imaginary);

	power = pow(real, 2) + pow(imaginary, 2);
}

void FourierUtils::complexToReal(fftw_complex& complex, double& real) {
	real = complex[0];
}

void FourierUtils::normalize1DSignal(fftw_complex* signal, fftw_complex* normalized, int signalSize) {
    // normalize result
    double multiplier = 1.0 / (double)signalSize;
    for (int i = 0; i < signalSize; ++i) {
    	FourierUtils::realTimesComplex(multiplier, signal[i], normalized[i]);
    }
}

void FourierUtils::complexToRealImaginary(fftw_complex& complex, double& real, double& imaginary) {
	complexToReal(complex, real);
	complexToImaginary(complex, imaginary);
}

void FourierUtils::complexToImaginary(fftw_complex& complex, double& imaginary) {
	imaginary = complex[1];
}

void FourierUtils::copyComplex(fftw_complex& original, fftw_complex& copy) {
	copy[0] = original[0];
	copy[1] = original[1];
}

void FourierUtils::realToComplex(double& real, fftw_complex& complex) {
	complex[0] = real;
}

void FourierUtils::justRealToComplex(double& real, fftw_complex& complex) {
	double imaginary = 0.0;
	realImaginaryToComplex(real, imaginary, complex);
}

void FourierUtils::realTimesComplex(double& real, fftw_complex& complex, fftw_complex& result) {
	double realPart, imaginaryPart;
	complexToReal(complex, realPart);
	realPart = real * realPart;

	complexToImaginary(complex, imaginaryPart);
	imaginaryPart = real * imaginaryPart;

	realImaginaryToComplex(realPart, imaginaryPart, result);
}

void FourierUtils::complexTimesComplex(fftw_complex& first, fftw_complex& second, fftw_complex& result) {
	double x, y, u, v, s, t;
	complexToReal(first, x);
	complexToImaginary(first, y);
	complexToReal(second, u);
	complexToReal(second, v);

	s = x * u - y * v;
	t = x * v + y * u;

	realImaginaryToComplex(s, t, result);
}

void FourierUtils::imaginaryToComplex(double& imaginary, fftw_complex& complex) {
	complex[1] = imaginary;
}

void FourierUtils::realImaginaryToComplex(double& real, double& imaginary, fftw_complex& complex) {
	realToComplex(real, complex);
	imaginaryToComplex(imaginary, complex);
}

