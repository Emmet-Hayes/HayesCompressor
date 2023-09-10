#pragma once

#include <JuceHeader.h>


class MeterNeedle : public juce::Component
{
public:
    MeterNeedle();
    void prepare(const float& s, const float& e);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void update(const float& val);
    void redrawNeedle(juce::Graphics& g, float centreX, float centreY, float length);
    void setMode(int m);
    void setNeedleEnabled(bool state);
private:
    juce::Rectangle<int> area;
    juce::Colour needleColour;
    float valueInDecibel;
    int mode;
    int minValue, maxValue;
    float sAngle, eAngle;
};
