/*
  ==============================================================================

    BQfilter.h
    Created: 10 Jan 2018 9:40:18am
    Author:  profw

  ==============================================================================
*/

#pragma once

#include "BQtools.h"

class BQfilter
{
private:
	// state variables
	double s1;
	double s2;
	// coefficients
	double b[3];
	double a[3];
public:
	BQfilter(void);
	BQfilter(FilterType, float, float, float, float);
	void setCoef(double[], double[]);
	void update(FilterType, float, float, float, float);
	void procblock(float[], int);
	void resetstate(void);
	double freqresp(double, float);
};