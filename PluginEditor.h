/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "FRdisplay.h"

//==============================================================================
/**
*/



class EqualizerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
	enum
	{
		TitleWidth = 300,
		TitleHeight = 50,
		SliderWidth = 80,
		SliderHeight = 200,
		SliderSpacing = 0,
		KnobWidth = 80,
		TextWidth = 40,
		TextHeight = 25,
		GraphHeight = 200,
		GraphWidth = 400,
		Margin = 50
	};
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    EqualizerAudioProcessorEditor (EqualizerAudioProcessor&, AudioProcessorValueTreeState&, SOSfilter*);
    ~EqualizerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	AudioProcessorValueTreeState& valueTreeState;

	Label GainLabel[EqualizerAudioProcessor::NumSects];
	Slider GainSlider[EqualizerAudioProcessor::NumSects];
	ScopedPointer<SliderAttachment> GainSliderAttachment[EqualizerAudioProcessor::NumSects];
	Label FrequencyLabel[EqualizerAudioProcessor::NumSects];
	Slider FrequencySlider[EqualizerAudioProcessor::NumSects];
	ScopedPointer<SliderAttachment> FrequencySliderAttachment[EqualizerAudioProcessor::NumSects];
	Label MasterGainLabel;
	Slider MasterGainSlider;
	ScopedPointer<SliderAttachment> MasterGainSliderAttachment;

	FRdisplay EQplot;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqualizerAudioProcessorEditor)
};
