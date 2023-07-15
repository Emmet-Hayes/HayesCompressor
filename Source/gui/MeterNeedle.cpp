#include "include/MeterNeedle.h"
#include "../util/include/Constants.h"

MeterNeedle::MeterNeedle()
{
    valueInDecibel = 0.0f;
    mode = 3;
    minValue = 0;
    maxValue = 0;
    sAngle = 0.0f;
    eAngle = 0.0f;
    needleColour = juce::Colour(155, 52, 155);
}

void MeterNeedle::paint(juce::Graphics& g)
{
    const auto bounds = area.toFloat();
    const float centreX = bounds.getX() + bounds.getWidth() * 0.5f;
    const float centreY = bounds.getY() + bounds.getHeight();
    const float needleLength = juce::jmin(bounds.getWidth() * 0.75f, bounds.getHeight() * 0.75f);

    g.setColour(needleColour);
    redrawNeedle(g, centreX, centreY, needleLength);
}

void MeterNeedle::resized()
{
    area = getLocalBounds().reduced(3);
    repaint();
}

void MeterNeedle::update(const float& val)
{
    if (val != valueInDecibel)
    {
        valueInDecibel = val;
        repaint();
    }
}

void MeterNeedle::redrawNeedle(juce::Graphics& g, float centreX, float centreY, float length)
{
    float val = std::clamp(valueInDecibel, static_cast<float>(minValue), static_cast<float>(maxValue));
    float mapped = juce::jmap(val, static_cast<float>(minValue), static_cast<float>(maxValue), sAngle, eAngle);
    mapped -= mapped > 2 * juce::MathConstants<float>::pi ? juce::MathConstants<float>::twoPi : 0.0f;
    const float x2 = centreX + sin(mapped) * length;
    const float y2 = centreY - cos(mapped) * length;
    g.drawArrow({centreX, centreY, x2, y2}, 2.0f, 0, 0);
}

void MeterNeedle::prepare(const float& s, const float& e)
{
    sAngle = s;
    eAngle = e;
}

void MeterNeedle::setMode(int m)
{
    minValue = m == 3 ? -30 : -50;
    mode = m;
    repaint();
}

void MeterNeedle::setNeedleEnabled(bool state)
{
    needleColour = state
                 ? juce::Colour(Constants::Colors::statusOutline)
                 : juce::Colour(Constants::Colors::statusOutline).withAlpha(0.35f);
    repaint();
}
