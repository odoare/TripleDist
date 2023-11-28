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
    auto lowg = apvts.getRawParameterValue("LowGain")->load();
    auto bandg = apvts.getRawParameterValue("BandGain")->load();
    auto highg = apvts.getRawParameterValue("HighGain")->load();
    auto lowl = apvts.getRawParameterValue("LowLevel")->load();
    auto bandl = apvts.getRawParameterValue("BandLevel")->load();
    auto highl = apvts.getRawParameterValue("HighLevel")->load();
    auto lowp = apvts.getRawParameterValue("LowPan")->load();
    auto bandp = apvts.getRawParameterValue("BandPan")->load();
    auto highp = apvts.getRawParameterValue("HighPan")->load();

    auto f = 2 * sin(3.1415927 * freq / SVFAudioProcessor::getSampleRate());
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        auto lp = ((1-channel)+(2*channel-1)*lowp)*lowg ;
        auto bp = ((1-channel)+(2*channel-1)*bandp)*bandg ;
        auto hp = ((1-channel)+(2*channel-1)*highp)*highg ;

        auto* channelData = buffer.getWritePointer (channel);

        for (int sample=0; sample<buffer.getNumSamples(); ++sample)
        {
            low[channel] = low[channel] + f * band[channel];
            high[channel] = channelData[sample] - low[channel] - q*band[channel];
            band[channel] = f * high[channel] + band[channel];
            channelData[sample] =  lowl*tanh(lp*low[channel])
                                    + bandl*tanh(bp*band[channel])
                                    + highl*tanh(hp*high[channel]);
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
    return new juce::GenericAudioProcessorEditor(*this);
    // return new SVFAudioProcessorEditor (*this);
}

//==============================================================================
void SVFAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SVFAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
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
    layout.add(std::make_unique<juce::AudioParameterFloat>("Q","Q",0.01f,1.0f,0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("LowGain","LowGain",0.0f,10.0f,1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BandGain","BandGain",0.0f,10.0f,1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("HighGain","HighGain",0.0f,10.0f,1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("LowLevel","LowLevel",0.0f,2.0f,1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BandLevel","BandLevel",0.0f,2.0f,1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("HighLevel","HighLevel",0.0f,2.0f,1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("LowPan","LowPan",0.0f,1.0f,0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BandPan","BandPan",0.0f,1.0f,0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("HighPan","HighPan",0.0f,1.0f,0.5f));

    return layout;
}