/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BQtools.h"

//==============================================================================
EqualizerAudioProcessor::EqualizerAudioProcessor() : parameters(*this, nullptr), sosL(NumSects), sosR(NumSects)
{
	// Parameters
	char paramID[10];
	char paramName[20];
	float alpha = pow(float(maxFreq) / float(minFreq), 1.0f / float(NumSects));
	float f0;
	float f1 = float(minFreq);
	//double a[3], b[3];

	for (int k = 0; k < NumSects; k++)
		filterType[k] = PEAK;
	filterType[0] = BASS;
	filterType[NumSects - 1] = TREBLE;

	for (int n = 0; n < NumSects; n++)
	{
		// Gain parameters
		sprintf(paramID, "Gain%d", n + 1);
		sprintf(paramName, "Channel %d gain", n + 1);
		parameters.createAndAddParameter(paramID, paramName, String(),
			NormalisableRange<float>(-20.0f, 20.0f, 0.1f), 0.0f, nullptr, nullptr);
		currentGain[n] = parameters.getRawParameterValue(paramID);
		// Frequency parameters
		sprintf(paramID, "Freq%d", n + 1);
		sprintf(paramName, "Channel %d freq", n + 1);
		f0 = f1;
		f1 = alpha * f0;
		parameters.createAndAddParameter(paramID, paramName, String(),
			NormalisableRange<float>(f0, f1, 1.0f), (f0 + f1) / 2.0f, nullptr, nullptr);
		currentFreq[n] = parameters.getRawParameterValue(paramID);
	}
	parameters.createAndAddParameter("Mgain", "Master Gain", String(),
		NormalisableRange<float>(-20.0f, 20.0f, 0.1f), 0.0f, nullptr, nullptr);
	currentMgain = parameters.getRawParameterValue("Mgain");

	parameters.state = ValueTree(Identifier("BQtreeState"));
	startTimerHz(5);
}

EqualizerAudioProcessor::~EqualizerAudioProcessor()
{
}

//==============================================================================
const String EqualizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EqualizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EqualizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EqualizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EqualizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EqualizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EqualizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EqualizerAudioProcessor::setCurrentProgram (int index)
{
}

const String EqualizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void EqualizerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void EqualizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	double b[3], a[3];

	for (int n = 0; n < NumSects; n++)
	{
		designbq(b, a, filterType[n], *currentGain[n], *currentFreq[n], Q, sampleRate);
		oldGain[n] = *currentGain[n];
		oldf0[n] = *currentFreq[n];
		sosL.updateSection(n, b, a);
		sosR.updateSection(n, b, a);
	}
	sosL.setSampRate(sampleRate);
	sosR.setSampRate(sampleRate);
}

void EqualizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EqualizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void EqualizerAudioProcessor::timerCallback()
{
	bool updateNeeded;
	double b[3], a[3];

	// Get current parameters and check for changes
	for (int n = 0; n < NumSects; n++)
	{
		updateNeeded = false;
		if (*currentGain[n] != oldGain[n])
		{
			updateNeeded = true;
			oldGain[n] = *currentGain[n];
		}
		if (*currentFreq[n] != oldf0[n])
		{
			updateNeeded = true;
			oldf0[n] = *currentFreq[n];
		}
		if (updateNeeded)
		{
			designbq(b, a, filterType[n], *currentGain[n], *currentFreq[n], Q, getSampleRate());
			sosL.updateSection(n, b, a);
			sosR.updateSection(n, b, a);
		}
	}
}

void EqualizerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

	// Clear excess output channels
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	// Process block of samples
	float * leftSamples = buffer.getWritePointer(0);
	float * rightSamples = buffer.getWritePointer(1);
	sosL.processBlock(leftSamples, buffer.getNumSamples());
	sosR.processBlock(rightSamples, buffer.getNumSamples());
	buffer.applyGain(pow(10.0f, *currentMgain / 20.0f));
}

//==============================================================================
bool EqualizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* EqualizerAudioProcessor::createEditor()
{
    return new EqualizerAudioProcessorEditor (*this, parameters, &sosL);
}

//==============================================================================
void EqualizerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	ScopedPointer<XmlElement> xml(parameters.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void EqualizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	ScopedPointer<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState != nullptr)
		if (xmlState->hasTagName(parameters.state.getType()))
			parameters.state = ValueTree::fromXml(*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EqualizerAudioProcessor();
}
