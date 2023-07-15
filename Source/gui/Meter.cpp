#include "include/Meter.h"
#include "../util/include/Constants.h"


Meter::Meter()
{
    //Init vars
    startAngle = static_cast<float>(5.0f / 3.0f * juce::MathConstants<float>::pi);
    endAngle = static_cast<float>(7.0f / 3.0f * juce::MathConstants<float>::pi);
    meterBg.prepare(startAngle, endAngle);
    meterBg.setMode(GR);
    needle.prepare(startAngle, endAngle);
    needle.setMode(GR);
    addAndMakeVisible(modeBox);
    modeBox.addItem("Input", 1);
    modeBox.addItem("Output", 2);
    modeBox.addItem("Gain Reduction", 3);
    modeBox.setJustificationType(juce::Justification::centred);
    modeBox.setSelectedId(3);
    modeBox.onChange = [this] { modeBoxChanged(); };
    addAndMakeVisible(modeBox);
    addAndMakeVisible(meterBg);
    addAndMakeVisible(needle);

    backgroundDarkGrey = juce::Colour(68, 24, 68);
}

void Meter::paint(juce::Graphics& g)
{
    g.setColour(backgroundDarkGrey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 3);
}

void Meter::resized()
{
    auto bounds = getLocalBounds();
    auto boxHeight = (bounds.getHeight() / 10) * 2;
    auto boxArea = bounds.removeFromTop(boxHeight);
    const juce::FlexItem::Margin comboMargin = juce::FlexItem::Margin(6.0f, 16.0f, 6.0f, 16.0f);

    juce::FlexBox mBox;
    mBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    mBox.flexDirection = juce::FlexBox::Direction::row;
    mBox.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    mBox.items.add(juce::FlexItem(modeBox).withFlex(1).withMargin(comboMargin));
    mBox.performLayout(boxArea.toFloat());

    meterBg.setBounds(bounds);
    needle.setBounds(bounds);
}

void Meter::setMode(int m)
{
    meterBg.setMode(m);
    needle.setMode(m);
    meterMode = m;
}

void Meter::modeBoxChanged()
{
    setMode(modeBox.getSelectedId());
}

void Meter::update(const float& val)
{
    if (val != valueInDecibel)
    {
        needle.update(val);
    }
}

int Meter::getMode()
{
    return meterMode;
}

float Meter::getValue()
{
    return valueInDecibel;
}

void Meter::setGUIEnabled(bool state)
{
    needle.setNeedleEnabled(state);
    meterBg.setIndicatorEnabled(state);
}
