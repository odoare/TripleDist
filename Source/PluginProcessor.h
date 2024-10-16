/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#define FOLEYS_SHOW_GUI_EDITOR_PALLETTE 0
#define NCHANNELS 2

#include <JuceHeader.h>

//==============================================================================
/**
*/
class TripleDistAudioProcessor  : public foleys::MagicProcessor
{
public:
    //==============================================================================
    TripleDistAudioProcessor();
    ~TripleDistAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    // juce::AudioProcessorEditor* createEditor() override;
    // bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    // void getStateInformation (juce::MemoryBlock& destData) override;
    // void setStateInformation (const void* data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameters();  
    juce::AudioProcessorValueTreeState apvts{*this,nullptr,"Parameters",createParameters()};

    // float getRmsLevel(const int bus, const int channel);
    // float getRmsLevelLow(const int channel);

private:

    float low[NCHANNELS],
          band[NCHANNELS],
          high[NCHANNELS];

    juce::AudioBuffer<float> lowBuffer,bandBuffer,highBuffer ;

    foleys::MagicLevelSource* inputLevel;
    foleys::MagicLevelSource* outputLevel;
    foleys::MagicLevelSource* lowLevel;
    foleys::MagicLevelSource* midLevel;
    foleys::MagicLevelSource* highLevel;
 
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TripleDistAudioProcessor)
};
