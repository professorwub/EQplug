/*
  ==============================================================================

    FRdisplay.cpp
    Created: 25 Apr 2018 8:21:13am
    Author:  profw

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FRdisplay.h"

//==============================================================================
FRdisplay::FRdisplay()
{
	maxFreq = 20000.0;
	maxDB = 20;
}

FRdisplay::FRdisplay(SOSfilter* sos)
{
	sosForPlot = sos;
	maxFreq = 20000.0;
	maxDB = 20;
	startTimerHz(30);
}

FRdisplay::~FRdisplay()
{
}

void FRdisplay::paint(Graphics& g)
{
	float x, y;
	Path freqRespMagPath;
	Line<float> gridLine;
	float dashLengths[] = { 2.0, 4.0 };
	char dbstr[16];
	char freqstr[16];

    g.fillAll (Colours::white);
	// Draw outline
    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 2);
	// Draw vertical grid lines
	x = getWidth() / 3.0f;
	g.drawLine(x, 0.0f, x, getHeight());
	g.drawLine(2.0f * x, 0.0f, 2.0f * x, getHeight());
	x *= log10f(5.0f);
	for (int n = 0; n < 3; n++)
	{
		gridLine.setStart(x, 0.0f);
		gridLine.setEnd(x, getHeight());
		g.drawDashedLine(gridLine, dashLengths, 2);
		x += getWidth() / 3.0f;
	}
	// Draw and label horizontal grid lines
	sprintf(dbstr, "%.0f dB", maxDB);
	Rectangle<int> numbox(5, 0, getWidth() - 10, getHeight() / 10);
	g.drawFittedText(dbstr, numbox, Justification::centred, 1);
	sprintf(dbstr, "-%.0f dB", maxDB);
	numbox.setY(getHeight() - getHeight() / 10);
	g.drawFittedText(dbstr, numbox, Justification::centred, 1);
	sprintf(freqstr, "%.0f Hz", maxFreq);
	g.drawFittedText(freqstr, numbox, Justification::right, 1);
	sprintf(freqstr, "%.0f Hz", maxFreq / 1000.0);
	g.drawFittedText(freqstr, numbox, Justification::left, 1);
	y = getHeight() / 4.0f;
	gridLine.setStart(0.0f, y);
	gridLine.setEnd(getWidth(), y);
	g.drawDashedLine(gridLine, dashLengths, 2);
	y += getHeight() / 4.0f;
	g.drawLine(0.0f, y, getWidth(), y);
	y += getHeight() / 4.0f;
	gridLine.setStart(0.0f, y);
	gridLine.setEnd(getWidth(), y);
	g.drawDashedLine(gridLine, dashLengths, 2);
	// Draw frequency response curve
	drawFR(freqRespMagPath);
	g.setColour(Colours::red);
	g.strokePath(freqRespMagPath, PathStrokeType(2.0));
}

void FRdisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void FRdisplay::timerCallback()
{
	this->repaint();
}

void FRdisplay::drawFR(Path& frpath)
{
	float x = 0.0;
	float xstep = getWidth() / float(numPoints);
	float db = 10.0 * log10(sosForPlot->freqResponse(maxFreq / 1000.0));
	frpath.clear();
	frpath.startNewSubPath(x, db2y(db));
	for (int n = 0; n < numPoints; n++)
	{
		x += xstep;
		db = 10.0 * log10(sosForPlot->freqResponse(x2freq(x)));
		frpath.lineTo(x, db2y(db));
	}
}

float FRdisplay::db2y(float frmag)
{
	return 0.5 * getHeight() * (1.0 - frmag / maxDB);
}

double FRdisplay::x2freq(float x)
{
	float alpha = 3.0 / getWidth();
	return (maxFreq * pow(10.0, alpha * x - 3.0));
}
