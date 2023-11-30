/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <iostream>
#include "Components/VerticalGradientMeter.h"
#include "Components/MyLookAndFeel.h"

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

    juce::Slider freqSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqSliderAttachment;
    juce::Slider qSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qSliderAttachment;

    juce::Slider inGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inGainSliderAttachment;
    juce::Slider outLevelSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outLevelSliderAttachment;

    juce::Slider lowGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowGainSliderAttachment;
    juce::Slider bandGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandGainSliderAttachment;
    juce::Slider highGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highGainSliderAttachment;
    
    juce::Slider lowLevelSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowLevelSliderAttachment;
    juce::Slider bandLevelSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandLevelSliderAttachment;
    juce::Slider highLevelSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highLevelSliderAttachment;

    juce::Slider lowPanSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowPanSliderAttachment;
    juce::Slider bandPanSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bandPanSliderAttachment;
    juce::Slider highPanSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highPanSliderAttachment;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SVFAudioProcessor& audioProcessor;

    MyLookAndFeel myLookAndFeelTeal{juce::Colours::teal.brighter()},
                  myLookAndFeelMagenta{juce::Colours::magenta},
                  myLookAndFeelBlue{juce::Colours::blue.brighter()},
                  myLookAndFeelGreen{juce::Colours::green.brighter()},
                  myLookAndFeelRed{juce::Colours::red.brighter()};

    Gui::VerticalGradientMeter verticalGradientMeterInL{[&]() { return audioProcessor.getRmsLevel(0,0); }},
                                verticalGradientMeterInR{[&]() { return audioProcessor.getRmsLevel(0,1); }},
                                verticalGradientMeterOutL{[&]() { return audioProcessor.getRmsLevel(1,0); }},
                                verticalGradientMeterOutR{[&]() { return audioProcessor.getRmsLevel(1,1); }},
                                verticalGradientMeterLowL{[&]() { return audioProcessor.getRmsLevel(2,0); }},
                                verticalGradientMeterLowR{[&]() { return audioProcessor.getRmsLevel(2,1); }},
                                verticalGradientMeterBandL{[&]() { return audioProcessor.getRmsLevel(3,0); }},
                                verticalGradientMeterBandR{[&]() { return audioProcessor.getRmsLevel(3,1); }},
                                verticalGradientMeterHighL{[&]() { return audioProcessor.getRmsLevel(4,0); }},
                                verticalGradientMeterHighR{[&]() { return audioProcessor.getRmsLevel(4,1); }};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SVFAudioProcessorEditor)
};
