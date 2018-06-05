/*
  ==============================================================================

    BQtools.cpp
    Created: 26 Apr 2018 1:27:11pm
    Author:  profw

  ==============================================================================
*/

#include "BQtools.h"

void designbq(double b[], double a[], FilterType ftype, float Gain, float f0, float Q, float fs)
{
	double gain = pow(10.0, abs(Gain) / 20.0);
	double pi = 2.0 * asin(1.0);
	double w0 = 2.0 * pi * f0;

	// compute analog filter coefficients
	double B[3];
	double A[3];
	A[0] = 1.0;
	switch (ftype)
	{
	case BASS:
		B[0] = 1.0;
		B[1] = 2.0 * sqrt(gain) * w0 / Q;
		B[2] = gain * pow(w0, 2.0);
		A[1] = 2.0 * w0 / Q;
		A[2] = pow(w0, 2.0);
		break;
	case TREBLE:
		B[0] = gain;
		B[1] = 2.0 * gain * w0 / Q;
		B[2] = gain * pow(w0, 2.0);
		A[1] = 2.0 * sqrt(gain) * w0 / Q;
		A[2] = gain * pow(w0, 2.0);
		break;
	case PEAK:
		B[0] = 1.0f;
		B[1] = gain * w0 / Q;
		B[2] = pow(w0, 2);
		A[1] = w0 / Q;
		A[2] = pow(w0, 2.0);
		break;
	default:
		break;
	}
	if (Gain < 0.0) // invert analog transfer function
	{
		double x;
		for (int n = 0; n < 3; n++)
		{
			x = B[n];
			B[n] = A[n];
			A[n] = x;
		}
	}
	// compute digital coefficients
	bilinear(B, A, b, a, f0, fs);
}

void bilinear(const double B[], const double A[], double b[], double a[], float f0, float fs)
{
	// Bilinear transform for BQ filter
	double pi = 2.0 * asin(1.0);
	double w0 = 2.0 * pi * f0;
	double K = w0 / tan(pi * f0 / fs);
	double D = A[0] * pow(K, 2.0) + A[1] * K + A[2];

	a[0] = 1.0;
	a[1] = 2.0 * (A[2] - A[0] * pow(K, 2.0)) / D;
	a[2] = (A[0] * pow(K, 2.0) - A[1] * K + A[2]) / D;
	b[0] = (B[0] * pow(K, 2.0) + B[1] * K + B[2]) / D;
	b[1] = 2.0f * (B[2] - B[0] * pow(K, 2.0)) / D;
	b[2] = (B[0] * pow(K, 2.0) - B[1] * K + B[2]) / D;
}