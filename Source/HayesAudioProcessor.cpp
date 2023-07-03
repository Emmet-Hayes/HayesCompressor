#include "HayesAudioProcessor.h"
#include "HayesAudioProcessorEditor.h"
#include "Utilities.h"

HayesAudioProcessor::HayesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
:   AudioProcessor (BusesProperties()
   #if ! JucePlugin_IsMidiEffect
    #if ! JucePlugin_IsSynth
     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
    #endif
     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
   #endif
)
,   apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

HayesAudioProcessor::~HayesAudioProcessor()
{
}

const juce::String HayesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HayesAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HayesAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HayesAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HayesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HayesAudioProcessor::getNumPrograms()
{
    return 1;
}

int HayesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HayesAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const juce::String HayesAudioProcessor::getProgramName (int /*index*/ )
{
    return {};
}

void HayesAudioProcessor::changeProgramName (int /*index*/, const juce::String& /*newName*/)
{
}

void HayesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 };
    compressor.prepare (spec);
}

void HayesAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HayesAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HayesAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{   
    auto threshold = static_cast<float>(*apvts.getRawParameterValue("threshold"));
    auto ratio = static_cast<float>(*apvts.getRawParameterValue("ratio"));
    auto attack = static_cast<float>(*apvts.getRawParameterValue("attack"));
    auto release = static_cast<float>(*apvts.getRawParameterValue("release"));

    compressor.setThreshold(threshold);
    compressor.setRatio(ratio);
    compressor.setAttack(attack);
    compressor.setRelease(release);
    
    juce::dsp::AudioBlock<float> block (buffer);
    compressor.process (juce::dsp::ProcessContextReplacing<float> (block));
}

bool HayesAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* HayesAudioProcessor::createEditor()
{
    return new HayesAudioProcessorEditor (*this);
}

void HayesAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.state.createXml())
        copyXmlToBinary(*xml, destData);
}

void HayesAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(apvts.state.getType()))
            apvts.state = juce::ValueTree::fromXml(*xml);
}

juce::AudioProcessorValueTreeState::ParameterLayout HayesAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("threshold", "Threshold", -48.0f, 1.0f, -12.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("ratio", "Ratio", 1.0f, 50.0f, 2.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("attack", "Attack", makeLogarithmicRange(1.0f, 5000.0f), 20.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("release", "Release", makeLogarithmicRange(1.0f, 5000.0f), 80.0f));
    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HayesAudioProcessor();
}
