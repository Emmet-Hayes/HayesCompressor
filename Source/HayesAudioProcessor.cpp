#include "HayesAudioProcessor.h"
#include "HayesAudioProcessorEditor.h"
#include "Utilities.h"

HayesAudioProcessor::HayesAudioProcessor()
:   apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

void HayesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 };
    compressor.prepare (spec);
}


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
