/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SVFAudioProcessor::SVFAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SVFAudioProcessor::~SVFAudioProcessor()
{
}

//==============================================================================
const juce::String SVFAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SVFAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SVFAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SVFAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SVFAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SVFAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SVFAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SVFAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SVFAudioProcessor::getProgramName (int index)
{
    return {};
}

void SVFAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SVFAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (unsigned int n=0;n<SVFAudioProcessor::nChannels;n++)
    {
        SVFAudioProcessor::low[n]=0.0f;
        SVFAudioProcessor::band[n]=0.0f;
        SVFAudioProcessor::high[n]=0.0f;
    }

    for (int channel=0;channel<nChannels;channel++)
    {
        rmsLevelIn[channel].reset(sampleRate,0.5);
        rmsLevelIn[channel].setCurrentAndTargetValue(-30.0f);
    }

    lowBuffer.setSize(2,samplesPerBlock);
    bandBuffer.setSize(2,samplesPerBlock);
    highBuffer.setSize(2,samplesPerBlock);

    // rmsLevelLowL.reset(sampleRate,0.5);
    // rmsLevelLowR.reset(sampleRate,0.5);
    // rmsLevelLowL.setCurrentAndTargetValue(-30.0f);
    // rmsLevelLowR.setCurrentAndTargetValue(-30.0f);
}

void SVFAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SVFAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void SVFAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto freq = apvts.getRawParameterValue("Frequency")->load();
    auto q = apvts.getRawParameterValue("Q")->load();
    auto ing = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("InGain")->load());
    auto outl = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("OutLevel")->load());
    auto lowg = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("LowGain")->load());
    auto bandg = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("BandGain")->load());
    auto highg = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("HighGain")->load());
    auto lowl = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("LowLevel")->load());
    auto bandl = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("BandLevel")->load());
    auto highl = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("HighLevel")->load());
    auto lowp = apvts.getRawParameterValue("LowPan")->load();
    auto bandp = apvts.getRawParameterValue("BandPan")->load();
    auto highp = apvts.getRawParameterValue("HighPan")->load();

    auto f = 2 * sin(juce::MathConstants<float>::pi * freq / SVFAudioProcessor::getSampleRate());
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        rmsLevelIn[channel].skip(buffer.getNumSamples());
        {
            const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(channel,0,buffer.getNumSamples()));
            if (value < rmsLevelIn[channel].getCurrentValue())
                rmsLevelIn[channel].setTargetValue(value);
            else
                rmsLevelIn[channel].setCurrentAndTargetValue(value);
        }

        auto lp = ((1-channel)+(2*channel-1)*lowp)*lowg ;
        auto bp = ((1-channel)+(2*channel-1)*bandp)*bandg ;
        auto hp = ((1-channel)+(2*channel-1)*highp)*highg ;

        auto* channelData = buffer.getWritePointer (channel);
        auto* lowData = lowBuffer.getWritePointer (channel);
        auto* bandData = bandBuffer.getWritePointer (channel);
        auto* highData = highBuffer.getWritePointer (channel);

        for (int sample=0; sample<buffer.getNumSamples(); ++sample)
        {
            low[channel] = low[channel] + f * band[channel];
            high[channel] = ing*channelData[sample] - low[channel] - q*band[channel];
            band[channel] = f * high[channel] + band[channel];
            lowData[sample] = lowl*tanh(lp*low[channel]);
            bandData[sample] = bandl*tanh(bp*band[channel]);
            highData[sample] = highl*tanh(hp*high[channel]);
            channelData[sample] =  outl*(lowData[sample]
                                    + bandData[sample]
                                    + highData[sample]);
        }
        rmsLevelOut[channel].skip(buffer.getNumSamples());
        {
            const auto value = juce::Decibels::gainToDecibels(buffer.getRMSLevel(channel,0,buffer.getNumSamples()));
            if (value < rmsLevelOut[channel].getCurrentValue())
                rmsLevelOut[channel].setTargetValue(value);
            else
                rmsLevelOut[channel].setCurrentAndTargetValue(value);
        }
        rmsLevelLow[channel].skip(lowBuffer.getNumSamples());
        {
            const auto value = juce::Decibels::gainToDecibels(lowBuffer.getRMSLevel(channel,0,lowBuffer.getNumSamples()));
            if (value < rmsLevelLow[channel].getCurrentValue())
                rmsLevelLow[channel].setTargetValue(value);
            else
                rmsLevelLow[channel].setCurrentAndTargetValue(value);
        }
        rmsLevelBand[channel].skip(bandBuffer.getNumSamples());
        {
            const auto value = juce::Decibels::gainToDecibels(bandBuffer.getRMSLevel(channel,0,bandBuffer.getNumSamples()));
            if (value < rmsLevelBand[channel].getCurrentValue())
                rmsLevelBand[channel].setTargetValue(value);
            else
                rmsLevelBand[channel].setCurrentAndTargetValue(value);
        }
        rmsLevelHigh[channel].skip(highBuffer.getNumSamples());
        {
            const auto value = juce::Decibels::gainToDecibels(highBuffer.getRMSLevel(channel,0,highBuffer.getNumSamples()));
            if (value < rmsLevelHigh[channel].getCurrentValue())
                rmsLevelHigh[channel].setTargetValue(value);
            else
                rmsLevelHigh[channel].setCurrentAndTargetValue(value);
        }
    }
}

//==============================================================================
bool SVFAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SVFAudioProcessor::createEditor()
{
    //return new juce::GenericAudioProcessorEditor(*this);
    return new SVFAudioProcessorEditor (*this);
}

//==============================================================================
void SVFAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void SVFAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data,sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
}

float SVFAudioProcessor::getRmsLevel(const int bus, const int channel)
{
    if (bus==0) return rmsLevelIn[channel].getCurrentValue();
    else if (bus==1) return rmsLevelOut[channel].getCurrentValue();
    else if (bus==2) return rmsLevelLow[channel].getCurrentValue();
    else if (bus==3) return rmsLevelBand[channel].getCurrentValue();
    else if (bus==4) return rmsLevelHigh[channel].getCurrentValue();
    else return 0.f;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SVFAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SVFAudioProcessor::createParameters()
{
    // std::vector<std::unique_ptr<juce::RangedAudioParameter>> params ;
    // params.push_back (std::make_unique<juce::AudioParameterFloat>("Gain","Gain",0.0f,1.0f,0.5f));
    // params.push_back (std::make_unique<juce::AudioParameterBool>("Phase","Phase",false));

    // return {params.begin(),params.end()};

    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("Frequency","Frequency",20.0f,10000.0f,1000.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Q","Q",0.02f,0.99f,0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("InGain","InGain",-90.0f,24.0f,0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("OutLevel","OutLevel",-90.0f,24.0f,0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("LowGain","LowGain",-90.0f,24.0f,0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BandGain","BandGain",-90.0f,24.0f,0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("HighGain","HighGain",-90.0f,24.0f,0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("LowLevel","LowLevel",-90.0f,24.0f,0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BandLevel","BandLevel",-90.0f,24.0f,0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("HighLevel","HighLevel",-90.0f,24.0f,0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("LowPan","LowPan",0.0f,1.0f,0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BandPan","BandPan",0.0f,1.0f,0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("HighPan","HighPan",0.0f,1.0f,0.5f));

    return layout;
}
