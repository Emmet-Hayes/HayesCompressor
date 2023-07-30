#include "HayesCompressorAudioProcessor.h"
#include "HayesCompressorAudioProcessorEditor.h"
#include "../../Common/Utilities.h"
#include "util/include/Constants.h"
#include <JuceHeader.h>

HayesCompressorAudioProcessor::HayesCompressorAudioProcessor()
:   BaseAudioProcessor(createParameterLayout())
{
    apvts.addParameterListener("power", this);
    apvts.addParameterListener("lookahead", this);
    apvts.addParameterListener("automakeup", this);
    apvts.addParameterListener("autoattack", this);
    apvts.addParameterListener("autorelease", this);
    apvts.addParameterListener("inputgain", this);
    apvts.addParameterListener("makeup", this);
    apvts.addParameterListener("threshold", this);
    apvts.addParameterListener("ratio", this);
    apvts.addParameterListener("knee", this);
    apvts.addParameterListener("attack", this);
    apvts.addParameterListener("release", this);
    apvts.addParameterListener("mix", this);

    gainReduction.set(0.0f);
    inputGain.set(-std::numeric_limits<float>::infinity());
    outputGain.set(-std::numeric_limits<float>::infinity());
}

void HayesCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec{ sampleRate,
                              static_cast<juce::uint32>(samplesPerBlock),
                              static_cast<juce::uint32>(getTotalNumOutputChannels()) };
    compressor.prepare(spec);
    inLevelFollower.prepare(sampleRate);
    outLevelFollower.prepare(sampleRate);
    inLevelFollower.setPeakDecay(0.3f);
    outLevelFollower.setPeakDecay(0.3f);

    if (*apvts.getRawParameterValue("lookahead") > 0.5f)
        setLatencySamples(static_cast<int>(0.005 * sampleRate));
    else
        setLatencySamples(0);
}


void HayesCompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{   
    juce::ScopedNoDenormals noDenormals;
    const auto totalNumInputChannels = getTotalNumInputChannels();
    const auto totalNumOutputChannels = getTotalNumOutputChannels();
    const auto numSamples = buffer.getNumSamples();

    // Clear buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    //Update input peak metering
    inLevelFollower.updatePeak(buffer.getArrayOfReadPointers(), totalNumInputChannels, numSamples);
    inputGain.set(juce::Decibels::gainToDecibels(inLevelFollower.getPeak()));

    // Do compressor processing
    compressor.process(buffer);

    // Update gain reduction metering
    gainReduction.set(compressor.getMaxGainReduction());

    // Update output peak metering
    outLevelFollower.updatePeak(buffer.getArrayOfReadPointers(), totalNumInputChannels, numSamples);
    outputGain = juce::Decibels::gainToDecibels(outLevelFollower.getPeak());
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


void HayesCompressorAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "inputgain") compressor.setInput(newValue);
    else if (parameterID == "power") compressor.setPower(!static_cast<bool>(newValue));
    else if (parameterID == "threshold") compressor.setThreshold(newValue);
    else if (parameterID == "automakeup") compressor.setAutoMakeup(static_cast<bool>(newValue));
    else if (parameterID == "autoattack")
    {
        const bool newBool = static_cast<bool>(newValue);
        compressor.setAutoAttack(newBool);
        if (!newBool) compressor.setAttack(*apvts.getRawParameterValue("attack"));
    }
    else if (parameterID == "autorelease")
    {
        const bool newBool = static_cast<bool>(newValue);
        compressor.setAutoRelease(newBool);
        if (!newBool) compressor.setRelease(*apvts.getRawParameterValue("release"));
    }
    else if (parameterID == "ratio") compressor.setRatio(newValue);
    else if (parameterID == "knee") compressor.setKnee(newValue);
    else if (parameterID == "attack") compressor.setAttack(newValue);
    else if (parameterID == "release") compressor.setRelease(newValue);
    else if (parameterID == "makeup") compressor.setMakeup(newValue);
    else if (parameterID == "lookahead")
    {
        const bool newBool = static_cast<bool>(newValue);
        if (newBool) setLatencySamples(static_cast<int>(0.005 * compressor.getSampleRate()));
        else setLatencySamples(0);
        compressor.setLookahead(newBool);
    }
    else if (parameterID == "mix") compressor.setMix(newValue);
}

juce::AudioProcessorValueTreeState::ParameterLayout HayesCompressorAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterBool>("lookahead", "Lookahead", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("automakeup", "AutoMakeup", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("autoattack", "AutoAttack", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("autorelease", "AutoRelease", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("power", "Power", true));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("inputgain", "Input",
        juce::NormalisableRange<float>(
            Constants::Parameter::inputStart,
            Constants::Parameter::inputEnd,
            Constants::Parameter::inputInterval), 0.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, float)
        {
            return juce::String(value, 1) + " dB";
        }));


    params.push_back(std::make_unique<juce::AudioParameterFloat>("threshold", "Thresh",
        juce::NormalisableRange<float>(
            Constants::Parameter::thresholdStart,
            Constants::Parameter::thresholdEnd,
            Constants::Parameter::thresholdInterval), -10.0f,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        [](float value, float /*maxStrLen*/)
        {
            return juce::String(value, 1) + " dB";
        }));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("ratio", "Ratio",
        juce::NormalisableRange<float>(
            Constants::Parameter::ratioStart,
            Constants::Parameter::ratioEnd,
            Constants::Parameter::ratioInterval, 0.5f), 2.0f,
        juce::String(), juce::AudioProcessorParameter::genericParameter,
        [](float value, float)
        {
            if (value > 23.9f)return juce::String("Infinity") + ":1";
            return juce::String(value, 1) + ":1";
        }));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("knee", "Knee",
        juce::NormalisableRange<float>(
            Constants::Parameter::kneeStart,
            Constants::Parameter::kneeEnd,
            Constants::Parameter::kneeInterval),
        6.0f, juce::String(), juce::AudioProcessorParameter::genericParameter,
        [](float value, float)
        {
            return juce::String(value, 1) + " dB";
        }));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("attack", "Attack",
        juce::NormalisableRange<float>(
            Constants::Parameter::attackStart,
            Constants::Parameter::attackEnd,
            Constants::Parameter::attackInterval, 0.5f), 2.0f,
        "ms",
        juce::AudioProcessorParameter::genericParameter,
        [](float value, float)
        {
            if (value == 100.0f) return juce::String(value, 0) + " ms";
            return juce::String(value, 2) + " ms";
        }));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("release", "Release",
        juce::NormalisableRange<float>(
            Constants::Parameter::releaseStart,
            Constants::Parameter::releaseEnd,
            Constants::Parameter::releaseInterval, 0.35f),
        140.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, float)
        {
            if (value <= 100) return juce::String(value, 2) + " ms";
            if (value >= 1000)
                return juce::String(value * 0.001f, 2) + " s";
            return juce::String(value, 1) + " ms";
        }));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("makeup", "Makeup",
        juce::NormalisableRange<float>(
            Constants::Parameter::makeupStart,
            Constants::Parameter::makeupEnd,
            Constants::Parameter::makeupInterval), 0.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, float)
        {
            return juce::String(value, 1) + " dB ";
        }));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("mix", "Mix",
        juce::NormalisableRange<float>(
            Constants::Parameter::mixStart,
            Constants::Parameter::mixEnd,
            Constants::Parameter::mixInterval),
        1.0f, "%", juce::AudioProcessorParameter::genericParameter,
        [](float value, float)
        {
            return juce::String(value * 100.0f, 1) + " %";
        }));

    return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HayesCompressorAudioProcessor();
}
