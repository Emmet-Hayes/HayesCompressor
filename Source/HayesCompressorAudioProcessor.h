#pragma once

#include "../../Common/BaseAudioProcessor.h"

#include "dsp/include/Compressor.h"
#include "dsp/include/LevelEnvelopeFollower.h"


class HayesCompressorAudioProcessor   : public BaseAudioProcessor
                                     , public juce::AudioProcessorValueTreeState::Listener
{
public:
    HayesCompressorAudioProcessor();

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() override;

    juce::Atomic<float> gainReduction;
    juce::Atomic<float> inputGain;
    juce::Atomic<float> outputGain;

private:
    Compressor compressor;
    LevelEnvelopeFollower inLevelFollower;
    LevelEnvelopeFollower outLevelFollower;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesCompressorAudioProcessor)
};
