#pragma once
#include <JuceHeader.h>
#include "dsp/include/Compressor.h"
#include "dsp/include/LevelEnvelopeFollower.h"

class HayesCompressorAudioProcessor  : public juce::AudioProcessor
                                     , public juce::AudioProcessorValueTreeState::Listener
{
public:
    HayesCompressorAudioProcessor();

    void releaseResources() override {}
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int /*index*/) override {}
    const juce::String getProgramName(int /*index*/) override { return {}; }
    void changeProgramName(int /*index*/, const juce::String& /*newName*/) override {}
    bool hasEditor() const override { return true; }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    juce::AudioProcessorValueTreeState apvts;

    juce::Atomic<float> gainReduction;
    juce::Atomic<float> inputGain;
    juce::Atomic<float> outputGain;
private:


    Compressor compressor;
    LevelEnvelopeFollower inLevelFollower;
    LevelEnvelopeFollower outLevelFollower;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesCompressorAudioProcessor)
};
