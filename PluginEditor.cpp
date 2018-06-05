/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EqualizerAudioProcessorEditor::EqualizerAudioProcessorEditor (EqualizerAudioProcessor& parent,
	AudioProcessorValueTreeState& vts, SOSfilter* sos)
    : AudioProcessorEditor (parent),
      valueTreeState (vts),
	  EQplot(sos)
{
	// Set up sliders
	char paramID[6];
	char labelName[12];
	int N = EqualizerAudioProcessor::NumSects;
	for (int n = 0; n < N; n++)
	{
		// Gain sliders and labels
		sprintf(labelName, "Ch %d Gain", n + 1);
		GainLabel[n].setText(labelName, dontSendNotification);
		GainLabel[n].setColour(Label::textColourId, Colours::black);
		addAndMakeVisible(GainLabel[n]);
		GainSlider[n].setSliderStyle(Slider::LinearVertical);
		GainSlider[n].setTextBoxStyle(Slider::TextBoxBelow, true, TextWidth, TextHeight);
		GainSlider[n].setColour(Slider::trackColourId, Colours::orangered);
		GainSlider[n].setColour(Slider::textBoxBackgroundColourId, Colours::white);
		GainSlider[n].setColour(Slider::textBoxTextColourId, Colours::black);
		addAndMakeVisible(GainSlider[n]);
		sprintf(paramID, "Gain%d", n + 1);
		GainSliderAttachment[n] = new SliderAttachment(valueTreeState, paramID, GainSlider[n]);
		// Frequency knobs and labels
		sprintf(labelName, "Ch %d Freq", n + 1);
		FrequencyLabel[n].setText(labelName, dontSendNotification);
		FrequencyLabel[n].setColour(Label::textColourId, Colours::black);
		addAndMakeVisible(FrequencyLabel[n]);
		FrequencySlider[n].setSliderStyle(Slider::Rotary);
		FrequencySlider[n].setTextBoxStyle(Slider::TextBoxBelow, true, TextWidth, TextHeight);
		FrequencySlider[n].setColour(Slider::rotarySliderFillColourId, Colours::orangered);
		FrequencySlider[n].setColour(Slider::textBoxBackgroundColourId, Colours::white);
		FrequencySlider[n].setColour(Slider::textBoxTextColourId, Colours::black);
		addAndMakeVisible(FrequencySlider[n]);
		sprintf(paramID, "Freq%d", n + 1);
		FrequencySliderAttachment[n] = new SliderAttachment(valueTreeState, paramID, FrequencySlider[n]);
	}
	// Master gain knob
	MasterGainLabel.setText("Master Gain", dontSendNotification);
	MasterGainLabel.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(MasterGainLabel);
	MasterGainSlider.setSliderStyle(Slider::Rotary);
	MasterGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, TextWidth, TextHeight);
	MasterGainSlider.setColour(Slider::rotarySliderFillColourId, Colours::orangered);
	MasterGainSlider.setColour(Slider::textBoxBackgroundColourId, Colours::white);
	MasterGainSlider.setColour(Slider::textBoxTextColourId, Colours::black);
	addAndMakeVisible(MasterGainSlider);
	MasterGainSliderAttachment = new SliderAttachment(valueTreeState, "Mgain", MasterGainSlider);

	addAndMakeVisible(EQplot);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (2 * Margin + N * SliderWidth + (N - 1) * SliderSpacing + KnobWidth,
		2 * Margin + TitleHeight + GraphHeight + SliderHeight + KnobWidth + 2 * TextHeight);
}

EqualizerAudioProcessorEditor::~EqualizerAudioProcessorEditor()
{
}

//==============================================================================
void EqualizerAudioProcessorEditor::paint (Graphics& g)
{
	Rectangle<int> titleRect(getWidth() / 2 - TitleWidth / 2, 0, TitleWidth, TitleHeight);

	g.fillAll(Colours::burlywood);
	g.setColour(Colours::black);
	g.setFont(24.0f);
	g.drawFittedText("Multiband Parametric Equalizer", titleRect, Justification::centred, 1);
}

void EqualizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	int x = Margin;
	int y = TitleHeight;
	EQplot.setBounds(x, y, GraphWidth, GraphHeight);
	for (int n = 0; n < EqualizerAudioProcessor::NumSects; n++)
	{
		y = TitleHeight + GraphHeight + Margin;
		GainSlider[n].setBounds(x, y, SliderWidth, SliderHeight);
		y += SliderHeight;
		GainLabel[n].setBounds(x, y, SliderWidth, TextHeight);
		y += TextHeight;
		FrequencySlider[n].setBounds(x, y, KnobWidth, KnobWidth);
		y += KnobWidth;
		FrequencyLabel[n].setBounds(x, y, SliderWidth, TextHeight);
		x += SliderWidth + SliderSpacing;
	}
	y = TitleHeight + GraphHeight + Margin + SliderHeight + TextHeight;
	MasterGainSlider.setBounds(x, y, KnobWidth, KnobWidth);
	y += KnobWidth;
	MasterGainLabel.setBounds(x, y, SliderWidth, TextHeight);
}
