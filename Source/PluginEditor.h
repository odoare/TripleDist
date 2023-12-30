/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <iostream>
#include "Components/VerticalMeter.h"
#include "Components/MyLookAndFeel.h"

//==============================================================================
/**
*/
class TripleDistAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TripleDistAudioProcessorEditor (TripleDistAudioProcessor&);
    ~TripleDistAudioProcessorEditor() override;

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
    TripleDistAudioProcessor& audioProcessor;

    MyLookAndFeel myLookAndFeelInGain{juce::Colours::teal.brighter(),"Gain"},
                  myLookAndFeelOutLevel{juce::Colours::teal.brighter(),"Level"},
                  myLookAndFeelLowGain{juce::Colours::blue.brighter(),"Drive"},
                  myLookAndFeelLowLevel{juce::Colours::blue.brighter(),"Level"},
                  myLookAndFeelLowPan{juce::Colours::blue.brighter(),"Pan"},
                  myLookAndFeelBandGain{juce::Colours::green.brighter(),"Drive"},
                  myLookAndFeelBandLevel{juce::Colours::green.brighter(),"Level"},
                  myLookAndFeelBandPan{juce::Colours::green.brighter(),"Pan"},
                  myLookAndFeelHighGain{juce::Colours::red.brighter(),"Drive"},
                  myLookAndFeelHighLevel{juce::Colours::red.brighter(),"Level"},
                  myLookAndFeelHighPan{juce::Colours::red.brighter(),"Pan"},
                  myLookAndFeelFreq{juce::Colours::magenta.brighter(),"Freq"},
                  myLookAndFeelQ{juce::Colours::magenta.brighter(),"Q"};

    Gui::VerticalMeter verticalMeterInL{[&]() { return audioProcessor.getRmsLevel(0,0); }},
                                verticalMeterInR{[&]() { return audioProcessor.getRmsLevel(0,1); }},
                                verticalMeterOutL{[&]() { return audioProcessor.getRmsLevel(1,0); }},
                                verticalMeterOutR{[&]() { return audioProcessor.getRmsLevel(1,1); }},
                                verticalMeterLowL{[&]() { return audioProcessor.getRmsLevel(2,0); }},
                                verticalMeterLowR{[&]() { return audioProcessor.getRmsLevel(2,1); }},
                                verticalMeterBandL{[&]() { return audioProcessor.getRmsLevel(3,0); }},
                                verticalMeterBandR{[&]() { return audioProcessor.getRmsLevel(3,1); }},
                                verticalMeterHighL{[&]() { return audioProcessor.getRmsLevel(4,0); }},
                                verticalMeterHighR{[&]() { return audioProcessor.getRmsLevel(4,1); }};

    juce::Image logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TripleDistAudioProcessorEditor)
};
