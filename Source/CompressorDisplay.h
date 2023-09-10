#pragma once

#include "HayesCompressorAudioProcessor.h"


class CompressorDisplay : public juce::Component, private juce::Timer
{
public:
    CompressorDisplay(HayesCompressorAudioProcessor& p) : processor(p)
    {
        startTimerHz(30);
    }

    void paint(juce::Graphics& g) override
    {
        //g.fillAll(juce::Colours::grey.withMultipliedAlpha(0.6));
        // get gain reduction from processor
        const float gainReduction = processor.gainReduction.get();

        // Let's use a color gradient to make it more visually appealing
        juce::ColourGradient gradient(juce::Colours::blue, 0, 0,
            juce::Colours::transparentBlack, 0, static_cast<float>(getHeight()),
            false);

        g.setGradientFill(gradient);

        // draw a simple meter using the gain reduction value
        const float height = juce::jmap(gainReduction, -60.0f, 0.0f, 1.0f, 0.0f) * getHeight();

        g.fillRect(0, getHeight() - static_cast<int>(height), getWidth(), static_cast<int>(height));
    }


    void timerCallback() override
    {
        repaint(); 
    }

private:
    HayesCompressorAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompressorDisplay)
};
