/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <iostream>

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
  void drawRotarySlider(juce::Graphics &g, 
          int x, int y, 
          int width,
          int height, 
          float sliderPos, 
          float rotaryStartAngle, 
          float rotaryEndAngle, 
          juce::Slider &slider) override
  {
    float diameter = juce::jmin(width,height);
    float radius = diameter * 0.5;
    float centreX = x + width * 0.5;
    float centreY = y + height * 0.5;
    float rx = centreX - radius;
    float ry = centreY - radius;
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle-rotaryStartAngle));

    juce::Rectangle<float> dialArea(rx,ry,diameter,diameter);
    g.setColour(juce::Colours::black);
    //g.drawRect(dialArea);
    g.fillEllipse(dialArea);
    g.setColour(juce::Colours::teal);
    g.drawEllipse(dialArea,2.0f);
    // g.drawText(juce::String());

    juce::Path dialTick;
    dialTick.addRectangle(0,-radius,10.0f,radius*0.33);
    g.fillPath(dialTick,juce::AffineTransform::rotation(angle).translated(centreX,centreY));
  };

  juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override
  {
    auto localBounds = slider.getLocalBounds().toFloat();
    juce::Slider::SliderLayout layout;
    layout.sliderBounds = slider.getLocalBounds();
    layout.textBoxBounds = slider.getLocalBounds().withSizeKeepingCentre (localBounds.getWidth() * 0.7f,
                                                                localBounds.getHeight() * 0.25f);
    return layout;
  }

  void drawLabel(juce::Graphics &g, juce::Label &label) override
  {
    g.setColour(juce::Colours::white);
    
    auto labelArea{label.getLocalBounds().toFloat()};
    g.setFont(labelArea.getHeight() * 0.9f);
    auto center = labelArea.getCentre();
   
    juce::String labelToDisplay = juce::String(label.getText());
    g.drawSingleLineText(labelToDisplay,
                          juce::roundToInt(center.x + g.getCurrentFont().getHorizontalScale()),
                          juce::roundToInt(center.y + g.getCurrentFont().getDescent()),
                          juce::Justification::horizontallyCentred);
    }
};

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

    MyLookAndFeel myLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SVFAudioProcessorEditor)
};
