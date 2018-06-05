/*
  ==============================================================================

    BQtools.h
    Created: 26 Apr 2018 1:27:11pm
    Author:  profw

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum FilterType { BASS, TREBLE, PEAK };


void designbq(double[], double[], FilterType, float, float, float, float);
void bilinear(const double[], const double[], double[], double[], float, float);