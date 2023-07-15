#pragma once
#include <JuceHeader.h>

class DelayLine
{
public:

    DelayLine() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);

    void process(juce::AudioBuffer<float>& buffer);

    void setDelay(float delayInSeconds);

private:
    void pushSamples(const juce::AudioBuffer<float>& srcBuffer, int nSam, int nCh);
    void readSamples(juce::AudioBuffer<float>& dstBuffer, int nSam, int nCh);

    inline void getReadBlockSize(int numSamples, int& index, int& block1, int& block2);
    inline void getWriteBlockSize(int numSamples, int& index, int& block1, int& block2);

private:
    //Directly initialize process spec to avoid debugging problems
    juce::dsp::ProcessSpec procSpec{-1, 0, 0};

    juce::AudioBuffer<float> delayBuffer;
    int delayBufferSize{0};
    int delayInSamples{0};
    float delayInSeconds{0.0f};
    int writePosition{0};
    bool isBypassed{true};
};
