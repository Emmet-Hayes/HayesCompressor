/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "HayesCompressorAudioProcessor.h"
#include "HayesCompressorAudioProcessorEditor.h"
#include "util/include/Constants.h"

//==============================================================================
HayesCompressorAudioProcessorEditor::HayesCompressorAudioProcessorEditor(HayesCompressorAudioProcessor& p)
:   BaseAudioProcessorEditor(p)
,   processor(p)
,   presetBar(p)
,   inGainLSlider(this), makeupGainLSlider(this), treshLSlider(this), ratioLSlider(this), kneeLSlider(this)
,   attackLSlider(this), releaseLSlider(this), mixLSlider(this)
,   powerButton("powerButton", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground)
{
    setLookAndFeel(&customLookAndFeel);
    initWidgets();
    setSize(400, 300);
    startTimerHz(60);
    image = juce::ImageCache::getFromMemory(BinaryData::bg_file_jpg, BinaryData::bg_file_jpgSize);
}

HayesCompressorAudioProcessorEditor::~HayesCompressorAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void HayesCompressorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 256, 800, 600);
}

void HayesCompressorAudioProcessorEditor::resized()
{
    int smallMargin = static_cast<int>(Constants::Margins::small);
    int mediumMargin = static_cast<int>(Constants::Margins::medium);
    int bigMargin = static_cast<int>(Constants::Margins::big);

    auto area = getLocalBounds().reduced(bigMargin);
    
    auto presetBarHeight = 15;
    presetBar.setBounds(area.removeFromTop(presetBarHeight));

    const auto headerHeight = presetBarHeight + 10;
    const auto btnAreaWidth = area.getWidth() / 5;
    const auto btnBotHeight = area.getHeight() / 3;

    auto header = area.removeFromTop(headerHeight).reduced(smallMargin);
    auto lBtnArea = area.removeFromLeft(btnAreaWidth).reduced(smallMargin);
    auto rBtnArea = area.removeFromRight(btnAreaWidth).reduced(smallMargin);
    auto botBtnArea = area.removeFromBottom(btnBotHeight).reduced(mediumMargin);

    const juce::FlexItem::Margin knobMargin = juce::FlexItem::Margin(Constants::Margins::medium);
    const juce::FlexItem::Margin knobMarginSmall = juce::FlexItem::Margin(Constants::Margins::medium);
    const juce::FlexItem::Margin buttonMargin = juce::FlexItem::Margin(Constants::Margins::small, Constants::Margins::big,
                                                                       Constants::Margins::small, Constants::Margins::big);

    juce::FlexBox headerBox;
    headerBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    headerBox.flexDirection = juce::FlexBox::Direction::row;
    headerBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    headerBox.items.add(juce::FlexItem(lahButton).withFlex(2).withMargin(buttonMargin));
    headerBox.items.add(juce::FlexItem(autoAttackButton).withFlex(2).withMargin(buttonMargin));
    headerBox.items.add(juce::FlexItem(autoReleaseButton).withFlex(2).withMargin(buttonMargin));
    headerBox.items.add(juce::FlexItem(autoMakeupButton).withFlex(2).withMargin(buttonMargin));
    headerBox.items.add(juce::FlexItem(powerButton).withFlex(1).withMargin(buttonMargin));
    headerBox.performLayout(header.toFloat());

    juce::FlexBox leftBtnBox;
    leftBtnBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    leftBtnBox.flexDirection = juce::FlexBox::Direction::column;
    leftBtnBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    leftBtnBox.items.add(juce::FlexItem(attackLSlider).withFlex(1).withMargin(knobMarginSmall));
    leftBtnBox.items.add(juce::FlexItem(releaseLSlider).withFlex(1).withMargin(knobMarginSmall));
    leftBtnBox.items.add(juce::FlexItem(inGainLSlider).withFlex(1).withMargin(knobMarginSmall));
    leftBtnBox.performLayout(lBtnArea.toFloat());

    juce::FlexBox rightBtnBox;
    rightBtnBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    rightBtnBox.flexDirection = juce::FlexBox::Direction::column;
    rightBtnBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    rightBtnBox.items.add(juce::FlexItem(kneeLSlider).withFlex(1).withMargin(knobMarginSmall));
    rightBtnBox.items.add(juce::FlexItem(ratioLSlider).withFlex(1).withMargin(knobMarginSmall));
    rightBtnBox.items.add(juce::FlexItem(mixLSlider).withFlex(1).withMargin(knobMarginSmall));
    rightBtnBox.performLayout(rBtnArea.toFloat());

    juce::FlexBox botBtnBox;
    botBtnBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    botBtnBox.flexDirection = juce::FlexBox::Direction::row;
    botBtnBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    botBtnBox.items.add(juce::FlexItem(treshLSlider).withFlex(1).withMargin(knobMargin));
    botBtnBox.items.add(juce::FlexItem(makeupGainLSlider).withFlex(1).withMargin(knobMargin));
    botBtnBox.performLayout(botBtnArea.toFloat());

    juce::FlexBox meterBox;
    meterBox.flexWrap = juce::FlexBox::Wrap::noWrap;
    meterBox.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    meterBox.items.add(juce::FlexItem(meter).withFlex(1).withMargin(Constants::Margins::big));
    meterBox.performLayout(area.toFloat());
}


void HayesCompressorAudioProcessorEditor::buttonClicked(juce::Button* b)
{
    if (b == &autoAttackButton)attackLSlider.setEnabled(!attackLSlider.isEnabled());
    if (b == &autoReleaseButton)releaseLSlider.setEnabled(!releaseLSlider.isEnabled());
    if (b == &powerButton) setGUIState(powerButton.getToggleState());
}

void HayesCompressorAudioProcessorEditor::timerCallback()
{
    int m = meter.getMode();
    switch (m)
    {
    case Meter::Mode::IN:
        meter.update(processor.inputGain.get());
        break;
    case Meter::Mode::OUT:
        meter.update(processor.outputGain.get());
        break;
    case Meter::Mode::GR:
        meter.update(processor.gainReduction.get());
        break;
    default:
        break;
    }
}

void HayesCompressorAudioProcessorEditor::initWidgets()
{
    addAndMakeVisible(inGainLSlider);
    inGainLSlider.reset(processor.apvts, "inputgain");
    inGainLSlider.setLabelText("Input");

    addAndMakeVisible(makeupGainLSlider);
    makeupGainLSlider.reset(processor.apvts, "makeup");
    makeupGainLSlider.setLabelText("Makeup");

    addAndMakeVisible(treshLSlider);
    treshLSlider.reset(processor.apvts, "threshold");
    treshLSlider.setLabelText("Threshold");

    addAndMakeVisible(ratioLSlider);
    ratioLSlider.reset(processor.apvts, "ratio");
    ratioLSlider.setLabelText("Ratio");

    addAndMakeVisible(kneeLSlider);
    kneeLSlider.reset(processor.apvts, "knee");
    kneeLSlider.setLabelText("Knee");

    addAndMakeVisible(attackLSlider);
    attackLSlider.reset(processor.apvts, "attack");
    attackLSlider.setLabelText("Attack");

    addAndMakeVisible(releaseLSlider);
    releaseLSlider.reset(processor.apvts, "release");
    releaseLSlider.setLabelText("Release");

    addAndMakeVisible(mixLSlider);
    mixLSlider.reset(processor.apvts, "mix");
    mixLSlider.setLabelText("Mix");

    addAndMakeVisible(lahButton);
    lahButton.setButtonText("LookAhead");
    lahButton.setClickingTogglesState(true);
    lahButton.setToggleState(false, juce::dontSendNotification);
    lahAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, "lookahead", lahButton));

    addAndMakeVisible(autoAttackButton);
    autoAttackButton.setButtonText("AutoAttack");
    autoAttackButton.setClickingTogglesState(true);
    autoAttackButton.setToggleState(false, juce::dontSendNotification);
    autoAttackButton.addListener(this);
    autoAttackAttachment.reset(
        new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, "autoattack", autoAttackButton));

    addAndMakeVisible(autoReleaseButton);
    autoReleaseButton.setButtonText("AutoRelease");
    autoReleaseButton.setClickingTogglesState(true);
    autoReleaseButton.setToggleState(false, juce::dontSendNotification);
    autoReleaseButton.addListener(this);
    autoReleaseAttachment.reset(
        new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, "autorelease", autoReleaseButton));

    addAndMakeVisible(autoMakeupButton);
    autoMakeupButton.setButtonText("Makeup");
    autoMakeupButton.setClickingTogglesState(true);
    autoMakeupButton.setToggleState(false, juce::dontSendNotification);
    autoMakeupButton.addListener(this);
    autoMakeupAttachment.reset(
        new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, "automakeup", autoMakeupButton));

    addAndMakeVisible(powerButton);
    powerButton.setImages(
        juce::Drawable::createFromImageData(BinaryData::power_white_svg, BinaryData::power_white_svgSize).get());
    powerButton.setClickingTogglesState(true);
    powerButton.setToggleState(true, juce::dontSendNotification);
    powerButton.addListener(this);
    powerAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, "power", powerButton));

    addAndMakeVisible(meter);
    meter.setMode(Meter::Mode::GR);
    
    addAndMakeVisible(presetBar);
}

void HayesCompressorAudioProcessorEditor::setGUIState(bool powerState)
{
    inGainLSlider.setEnabled(powerState);
    treshLSlider.setEnabled(powerState);
    ratioLSlider.setEnabled(powerState);
    kneeLSlider.setEnabled(powerState);
    makeupGainLSlider.setEnabled(powerState);
    mixLSlider.setEnabled(powerState);
    meter.setEnabled(powerState);
    meter.setGUIEnabled(powerState);
    lahButton.setEnabled(powerState);
    autoMakeupButton.setEnabled(powerState);

    autoAttackButton.setEnabled(powerState);
    autoReleaseButton.setEnabled(powerState);

    if (!powerState)
    {
        attackLSlider.setEnabled(powerState);
        releaseLSlider.setEnabled(powerState);
    }
    else
    {
        attackLSlider.setEnabled(!autoAttackButton.getToggleState());
        releaseLSlider.setEnabled(!autoReleaseButton.getToggleState());
    }
}
