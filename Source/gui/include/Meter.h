#pragma once

#include <JuceHeader.h>

#include "MeterNeedle.h"
#include "MeterBackground.h"


class Meter : public juce::Component
{
public:
    enum Mode { IN = 1, OUT, GR };

    Meter();
    void paint(juce::Graphics& g) override;
    void resized() override;
    void setMode(int m);
    void modeBoxChanged();
    void update(const float& val);
    int getMode();
    float getValue();
    void setGUIEnabled(bool state);
private:
    MeterBackground meterBg;
    MeterNeedle needle;
    juce::ComboBox modeBox;
    juce::Colour backgroundDarkGrey;
    int meterMode;
    float valueInDecibel;
    float startAngle, endAngle;
};
