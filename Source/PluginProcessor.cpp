/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

//==============================================================================
TripleDistAudioProcessor::TripleDistAudioProcessor()
{
    magicState.setGuiValueTree (BinaryData::magic_xml, BinaryData::magic_xmlSize);
    FOLEYS_SET_SOURCE_PATH("/home/od/src/Projects/TripleDist/Resources/magic.xml");
    inputLevel = magicState.createAndAddObject<foleys::MagicLevelSource>("inputLevel");
    outputLevel = magicState.createAndAddObject<foleys::MagicLevelSource>("outputLevel");
    lowLevel = magicState.createAndAddObject<foleys::MagicLevelSource>("lowLevel");
    midLevel = magicState.createAndAddObject<foleys::MagicLevelSource>("midLevel");
    highLevel = magicState.createAndAddObject<foleys::MagicLevelSource>("highLevel");
}

TripleDistAudioProcessor::~TripleDistAudioProcessor()
{
}

//==============================================================================
const juce::String TripleDistAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TripleDistAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TripleDistAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TripleDistAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TripleDistAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TripleDistAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TripleDistAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TripleDistAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TripleDistAudioProcessor::getProgramName (int index)
{
    return {};
}

void TripleDistAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TripleDistAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (unsigned int n=0;n<NCHANNELS;n++)
    {
        TripleDistAudioProcessor::low[n]=0.0f;
        TripleDistAudioProcessor::band[n]=0.0f;
        TripleDistAudioProcessor::high[n]=0.0f;
    }

    lowBuffer.setSize(NCHANNELS,samplesPerBlock);
    bandBuffer.setSize(NCHANNELS,samplesPerBlock);
    highBuffer.setSize(NCHANNELS,samplesPerBlock);

    inputLevel->setupSource(NCHANNELS,sampleRate,100);
    outputLevel->setupSource(NCHANNELS,sampleRate,100);
    lowLevel->setupSource(NCHANNELS,sampleRate,100);
    midLevel->setupSource(NCHANNELS,sampleRate,100);
    highLevel->setupSource(NCHANNELS,sampleRate,100);
}

void TripleDistAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool TripleDistAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo()
     || layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void TripleDistAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

    auto f = 2 * sin(juce::MathConstants<float>::pi * freq / TripleDistAudioProcessor::getSampleRate());
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    inputLevel->pushSamples(buffer);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
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
    }
    lowLevel->pushSamples(lowBuffer);
    midLevel->pushSamples(bandBuffer);
    highLevel->pushSamples(highBuffer);  
    outputLevel->pushSamples(buffer);
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TripleDistAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout TripleDistAudioProcessor::createParameters()
{

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