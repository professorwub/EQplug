/*
  ==============================================================================

    FRdisplay.h
    Created: 25 Apr 2018 8:21:13am
    Author:  profw

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SOSfilter.h"

//==============================================================================
/*
*/
class FRdisplay    : public Component, private Timer
{
public:
	enum {
		numPoints = 100
	};

    FRdisplay();
	FRdisplay(SOSfilter*);
    ~FRdisplay();

    void paint (Graphics&) override;
    void resized() override;
	void timerCallback() override;

	void drawFR(Path&);

	float db2y(float);
	double x2freq(float);

private:
	double maxFreq;
	float maxDB;

	SOSfilter* sosForPlot;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FRdisplay)
};
