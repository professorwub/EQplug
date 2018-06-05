/*
  ==============================================================================

    BQfilter.cpp
    Created: 10 Jan 2018 9:40:18am
    Author:  profw

  ==============================================================================
*/

#include "BQfilter.h"

BQfilter::BQfilter(void)
{
	// initialize state
	s1 = 0.0;
	s2 = 0.0;
}

BQfilter::BQfilter(FilterType ftype, float Gain, float f0, float Q, float fs)
{
	// compute filter coefficients
	designbq(b, a, ftype, Gain, f0, Q, fs);
	// initialize state
	s1 = 0.0;
	s2 = 0.0;
}
void BQfilter::setCoef(double num[], double den[])
{
	for (int n = 0; n < 3; n++)
	{
		a[n] = den[n];
		b[n] = num[n];
	}
}

void BQfilter::update(FilterType ftype, float Gain, float f0, float Q, float fs)
{
	// (re)compute filter coefficients
	designbq(b, a, ftype, Gain, f0, Q, fs);
}

void BQfilter::procblock(float x[], int N)
{
	float y;
	for (int n = 0; n < N; n++)
	{
		y = b[0] * x[n] + s1;
		s1 = s2 + b[1] * x[n] - a[1] * y;
		s2 = b[2] * x[n] - a[2] * y;
		x[n] = y;
	}
}

void BQfilter::resetstate(void)
{
	s1 = 0.0;
	s2 = 0.0;
}

double BQfilter::freqresp(double freq, float fsamp)
{
	// Compute magnitude of frequency response at frequency theta (radians/sample)
	// Double precision apparently necessary
	double h;
	double pi = 2.0 * asin(1.0);
	double B0 = pow(b[0], 2.0) + pow(b[1], 2.0) + pow(b[2], 2.0);
	double B1 = 2.0 * (b[0] * b[1] + b[1] * b[2]);
	double B2 = 2.0 * b[0] * b[2];
	double A0 = 1.0 + pow(a[1], 2.0) + pow(a[2], 2.0);
	double A1 = 2.0 * (a[1] + a[1] * a[2]);
	double A2 = 2.0 * a[2];
	double theta = 2.0 * pi * freq / fsamp;

	h = abs((B0 + B1 * cos(theta) + B2 * cos(2.0 * theta)) / (A0 + A1 * cos(theta) + A2 * cos(2.0 * theta)));
	return (isnan(h) ? 1.0 : h); // in case of 0.0 / 0.0
}