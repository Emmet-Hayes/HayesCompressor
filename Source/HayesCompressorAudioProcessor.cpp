#include "HayesCompressorAudioProcessor.h"
#include "HayesCompressorAudioProcessorEditor.h"
#include "Utilities.h"

HayesCompressorAudioProcessor::HayesCompressorAudioProcessor()
:   apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

void HayesCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    const juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 };
    compressor.prepare (spec);
}


void HayesCompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{   
    auto threshold = static_cast<float>(*apvts.getRawParameterValue("threshold"));
    auto ratio     = static_cast<float>(*apvts.getRawParameterValue("ratio"));
    auto attack    = static_cast<float>(*apvts.getRawParameterValue("attack"));
    auto release   = static_cast<float>(*apvts.getRawParameterValue("release"));
    
    compressor.setThreshold(threshold);
    compressor.setRatio(ratio);
    compressor.setAttack(attack);
    compressor.setRelease(release);
    
    buffer.applyGain(juce::Decibels::decibelsToGain(inputGain.get()));
    
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    compressor.process(context);
    
    buffer.applyGain(juce::Decibels::decibelsToGain(outputGain.get()));
}


juce::AudioProcessorEditor* HayesCompressorAudioProcessor::createEditor()
{
    return new HayesCompressorAudioProcessorEditor(*this);
}

void HayesCompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.state.createXml())
        copyXmlToBinary(*xml, destData);
}

void HayesCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
        if (xml->hasTagName(apvts.state.getType()))
            apvts.state = juce::ValueTree::fromXml(*xml);
}

juce::AudioProcessorValueTreeState::ParameterLayout HayesCompressorAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("inputgain", "Input Gain", -36.0f, 24.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("threshold", "Threshold", -48.0f, 1.0f, -12.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("ratio", "Ratio", 1.0f, 50.0f, 2.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("attack", "Attack", makeLogarithmicRange(1.0f, 5000.0f), 20.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("release", "Release", makeLogarithmicRange(1.0f, 5000.0f), 80.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("outputgain", "Output Gain", -48.0f, 12.0f, 0.0f));
    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HayesCompressorAudioProcessor();
}
