#pragma once
#include <JuceHeader.h>

class MeterBackground : public juce::Component
{
public:

    MeterBackground();
    void prepare(const float& s, const float& e);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void drawIndicators(juce::Graphics& g, float centreX, float centreY, float length);
    void setMode(int m);
    void setIndicatorEnabled(bool state);
private:
    juce::Rectangle<int> meterArea;
    juce::Colour indicatorColour;
    juce::Colour backgroundApp;
    int mode;
    int minValue, maxValue;
    float sAngle, eAngle;
    int step;
};
