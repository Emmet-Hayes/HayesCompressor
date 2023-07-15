#pragma once
#include <JuceHeader.h>

class SliderPopupLabel : public juce::Label
{
public:
    SliderPopupLabel(juce::Slider& s)
    :   slider { s } 
    {}

    void paint(juce::Graphics& g) override
    {
        // Fill in with your own drawing code
        g.fillAll(juce::Colours::lightgrey);
        g.setColour(juce::Colours::black);
        g.drawText(slider.getTextFromValue(slider.getValue()), getLocalBounds(), juce::Justification::centred, true);
    }

private:
    juce::Slider& slider;
};