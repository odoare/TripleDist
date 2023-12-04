/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SVFAudioProcessorEditor::SVFAudioProcessorEditor (SVFAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    freqSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow,false,0,0);
    freqSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(freqSlider);
    freqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Frequency",freqSlider);
    freqSlider.setLookAndFeel(&myLookAndFeelFreq);

    qSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    qSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(qSlider);
    qSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Q",qSlider);
    qSlider.setLookAndFeel(&myLookAndFeelQ);

    inGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    inGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    inGainSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(inGainSlider);
    inGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"InGain",inGainSlider);
    inGainSlider.setLookAndFeel(&myLookAndFeelInGain);

    outLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    outLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    outLevelSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(outLevelSlider);
    outLevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"OutLevel",outLevelSlider);
    outLevelSlider.setLookAndFeel(&myLookAndFeelOutLevel);

    lowGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    lowGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    lowGainSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(lowGainSlider);
    lowGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"LowGain",lowGainSlider);
    lowGainSlider.setLookAndFeel(&myLookAndFeelLowGain);

    bandGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    bandGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    bandGainSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(bandGainSlider);
    bandGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BandGain",bandGainSlider);
    bandGainSlider.setLookAndFeel(&myLookAndFeelBandGain);

    highGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    highGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    highGainSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(highGainSlider);
    highGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"HighGain",highGainSlider);
    highGainSlider.setLookAndFeel(&myLookAndFeelHighGain);

    lowLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    lowLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    lowLevelSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(lowLevelSlider);
    lowLevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"LowLevel",lowLevelSlider);
    lowLevelSlider.setLookAndFeel(&myLookAndFeelLowLevel);
    
    bandLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    bandLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    bandLevelSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(bandLevelSlider);
    bandLevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BandLevel",bandLevelSlider);
    bandLevelSlider.setLookAndFeel(&myLookAndFeelBandLevel);

    highLevelSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    highLevelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    highLevelSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(highLevelSlider);
    highLevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"HighLevel",highLevelSlider);
    highLevelSlider.setLookAndFeel(&myLookAndFeelHighLevel);

    lowPanSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    lowPanSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    lowPanSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(lowPanSlider);
    lowPanSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"LowPan",lowPanSlider);
    lowPanSlider.setLookAndFeel(&myLookAndFeelLowPan);

    bandPanSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    bandPanSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    bandPanSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(bandPanSlider);
    bandPanSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"BandPan",bandPanSlider);
    bandPanSlider.setLookAndFeel(&myLookAndFeelBandPan);

    highPanSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    highPanSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,100,20);
    highPanSlider.setPopupDisplayEnabled(true,true,nullptr,1000);
    addAndMakeVisible(highPanSlider);
    highPanSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"HighPan",highPanSlider);
    highPanSlider.setLookAndFeel(&myLookAndFeelHighPan);

    verticalMeterInL.setColour(juce::Colours::teal.brighter());
    addAndMakeVisible(verticalMeterInL);
    verticalMeterInR.setColour(juce::Colours::teal.brighter());
    addAndMakeVisible(verticalMeterInR);

    verticalMeterOutL.setColour(juce::Colours::teal.brighter());
    addAndMakeVisible(verticalMeterOutL);
    verticalMeterOutR.setColour(juce::Colours::teal.brighter());
    addAndMakeVisible(verticalMeterOutR);

    verticalMeterLowL.setColour(juce::Colours::blue.brighter());
    addAndMakeVisible(verticalMeterLowL);
    verticalMeterLowR.setColour(juce::Colours::blue.brighter());
    addAndMakeVisible(verticalMeterLowR);
    
    verticalMeterBandL.setColour(juce::Colours::green.brighter());
    addAndMakeVisible(verticalMeterBandL);
    verticalMeterBandR.setColour(juce::Colours::green.brighter());
    addAndMakeVisible(verticalMeterBandR);

    verticalMeterHighL.setColour(juce::Colours::red.brighter());
    addAndMakeVisible(verticalMeterHighL);
    verticalMeterHighR.setColour(juce::Colours::red.brighter());
    addAndMakeVisible(verticalMeterHighR);
    setSize (600, 400);
    setResizable(true,true);
}

SVFAudioProcessorEditor::~SVFAudioProcessorEditor()
{
}

//==============================================================================
void SVFAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    float border = 0.05 ;
    float unitXBorder = border*getWidth();
    float unitYBorder = border*getHeight();
    auto unitX = (1-2*border)*getWidth()/6;
    auto unitY = (1-2*border)*getHeight()/4;
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll (juce::Colour::fromFloatRGBA (0.1f, 0.1f, 0.15f, 1.0f));
    // g.fillAll (juce::Colours::black);
    g.setColour(juce::Colours::grey.brighter());
    g.setFont(44);
    g.drawSingleLineText("Triple Dist",
                          unitXBorder+3.9*unitX,
                          unitYBorder+unitY/2,
                          juce::Justification::horizontallyCentred);
    g.setFont(16);
    g.drawSingleLineText("odiomechanics.com",
                          unitXBorder+3.9*unitX,
                          unitYBorder+1.5*unitY/2,
                          juce::Justification::horizontallyCentred);

    int fontSize = 18;
    g.setFont (fontSize);
    g.drawMultiLineText ("IN", unitXBorder+unitX/2-fontSize/3 , 4*unitYBorder, fontSize/2, juce::Justification::centred);
    g.drawMultiLineText ("OUT", unitXBorder+5*unitX+unitX/2-fontSize/3 , 3*unitYBorder, fontSize/2, juce::Justification::centred);
    g.drawMultiLineText ("LOW", unitXBorder+unitX+unitX/2-fontSize/3 , 3*unitYBorder+3*unitY, fontSize/2, juce::Justification::centred);
    g.drawMultiLineText ("MID", unitXBorder+3*unitX+unitX/2-fontSize/3 , 3*unitYBorder+3*unitY, fontSize/2, juce::Justification::centred);
    g.drawMultiLineText ("HIGH", unitXBorder+5*unitX+unitX/2-fontSize/3 , 2*unitYBorder+3*unitY, fontSize/2, juce::Justification::centred);
}

void SVFAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    float border = 0.05 ;
    float unitXBorder = border*getWidth();
    float unitYBorder = border*getHeight();
    auto unitX = (1-2*border)*getWidth()/6;
    auto unitY = (1-2*border)*getHeight()/4;
    
    qSlider.setBounds(unitXBorder+2.5*unitX,unitYBorder+unitY,unitX,unitY);
    freqSlider.setBounds(unitXBorder+unitX,unitYBorder,2*unitX,2*unitY);

    inGainSlider.setBounds(unitXBorder,unitYBorder+unitY,unitX,unitY);
    outLevelSlider.setBounds(unitXBorder+5*unitX,unitYBorder+unitY,unitX,unitY);

    lowGainSlider.setBounds(unitXBorder,unitYBorder+2*unitY,unitX,unitY);
    bandGainSlider.setBounds(unitXBorder+2*unitX,unitYBorder+2*unitY,unitX,unitY);
    highGainSlider.setBounds(unitXBorder+4*unitX,unitYBorder+2*unitY,unitX,unitY);
    lowLevelSlider.setBounds(unitXBorder+unitX,unitYBorder+2*unitY,unitX,unitY);
    bandLevelSlider.setBounds(unitXBorder+3*unitX,unitYBorder+2*unitY,unitX,unitY);
    highLevelSlider.setBounds(unitXBorder+5*unitX,unitYBorder+2*unitY,unitX,unitY);
    lowPanSlider.setBounds(unitXBorder,unitYBorder+3*unitY,unitX,unitY);
    bandPanSlider.setBounds(unitXBorder+2*unitX,unitYBorder+3*unitY,unitX,unitY);
    highPanSlider.setBounds(unitXBorder+4*unitX,unitYBorder+3*unitY,unitX,unitY);

    verticalMeterInL.setBounds(unitXBorder+0.5*unitX/3,unitYBorder,unitX/3,unitY);
    verticalMeterInR.setBounds(unitXBorder+unitX/2,unitYBorder,unitX/3,unitY);

    verticalMeterOutL.setBounds(unitXBorder+5*unitX+0.5*unitX/3,unitYBorder,unitX/3,unitY);
    verticalMeterOutR.setBounds(unitXBorder+5*unitX+unitX/2,unitYBorder,unitX/3,unitY);

    verticalMeterLowL.setBounds(unitXBorder+unitX+0.5*unitX/3,unitYBorder+3*unitY,unitX/3,unitY);
    verticalMeterLowR.setBounds(unitXBorder+unitX+unitX/2,unitYBorder+3*unitY,unitX/3,unitY);
    verticalMeterBandL.setBounds(unitXBorder+3*unitX+0.5*unitX/3,unitYBorder+3*unitY,unitX/3,unitY);
    verticalMeterBandR.setBounds(unitXBorder+3*unitX+unitX/2,unitYBorder+3*unitY,unitX/3,unitY);
    verticalMeterHighL.setBounds(unitXBorder+5*unitX+0.5*unitX/3,unitYBorder+3*unitY,unitX/3,unitY);
    verticalMeterHighR.setBounds(unitXBorder+5*unitX+unitX/2,unitYBorder+3*unitY,unitX/3,unitY);
}
