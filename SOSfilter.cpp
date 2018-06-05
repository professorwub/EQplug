/*
  ==============================================================================

    SOSfilter.cpp
    Created: 26 Apr 2018 8:29:28am
    Author:  profw

  ==============================================================================
*/

#include "SOSfilter.h"

SOSfilter::SOSfilter()
{
}

SOSfilter::SOSfilter(int n)
{
	numSects = n;
	filterSection = new BQfilter[n];
}

SOSfilter::~SOSfilter()
{
	delete[] filterSection;
}

void SOSfilter::setSampRate(float fs)
{
	sampRate = fs;
}

void SOSfilter::updateSection(int sectNo, double num[], double den[])
{
	filterSection[sectNo].setCoef(num, den);
}

void SOSfilter::processBlock(float x[], int N)
{
	for (int n = 0; n < numSects; n++)
	{
		filterSection[n].procblock(x, N);
	}
}
double SOSfilter::freqResponse(double freq)
{
	// Compute magnitude of frequency response at frequency theta (radians/sample)
	double frmag = 1.0;

	for (int n = 0; n < numSects; n++)
	{
		frmag *= filterSection[n].freqresp(freq, sampRate);
	}
	return frmag;
}