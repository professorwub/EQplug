/*
  ==============================================================================

    SOSfilter.h
    Created: 26 Apr 2018 8:29:28am
    Author:  profw

  ==============================================================================
*/

#pragma once

#include "BQfilter.h"

class SOSfilter
{
public:
	SOSfilter();
	SOSfilter(int);
	~SOSfilter();
	void setSampRate(float);
	void updateSection(int, double[], double[]);
	void processBlock(float[], int);
	double freqResponse(double);
private:
	int numSects;
	BQfilter* filterSection;
	float sampRate = 44100.0f; // just in case
};