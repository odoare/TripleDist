/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SVFAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SVFAudioProcessorEditor (SVFAudioProcessor&);
    ~SVFAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    // juce::Slider freqSlider;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqSliderAttachment;
    // juce::Slider qSlider;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qSliderAttachment;
    // juce::Slider lowLevelSlider;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowLevelSliderAttachment;
    // juce::Slider bandLevelSlider;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandLevelSliderAttachment;
    // juce::Slider highLevelSlider;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highLevelSliderAttachment;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SVFAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SVFAudioProcessorEditor)
};
