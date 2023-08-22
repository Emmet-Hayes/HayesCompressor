
#include "HayesCompressorAudioProcessor.h"
#include "HayesCompressorAudioProcessorEditor.h"
#include "util/include/Constants.h"


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
    startTimerHz(60);
    image = juce::ImageCache::getFromMemory(BinaryData::bg_file_jpg, BinaryData::bg_file_jpgSize);
  
    const auto ratio = static_cast<double> (defaultWidth) / defaultHeight;
    setResizable(false, true);
    getConstrainer()->setFixedAspectRatio(ratio);
    getConstrainer()->setSizeLimits(defaultWidth, defaultHeight, defaultWidth * 2, defaultHeight * 2);
    setSize(defaultWidth, defaultHeight);
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
    const auto scale = static_cast<float> (getWidth()) / defaultWidth;

    auto setBoundsAndApplyScaling = [&](juce::Component* component, int x, int y, int w, int h)
    {
        component->setBounds(static_cast<int>(x * scale), static_cast<int>(y * scale),
            static_cast<int>(w * scale), static_cast<int>(h * scale));
    };
    
    customLookAndFeel.setWindowScale(scale);
    setBoundsAndApplyScaling(&presetBar, 0, 0, 400, 20);
    setBoundsAndApplyScaling(&inGainLSlider, 175, 30, 70, 70);
    setBoundsAndApplyScaling(&makeupGainLSlider, 265, 30, 70, 70);
    setBoundsAndApplyScaling(&treshLSlider, 15, 30, 80, 80);
    setBoundsAndApplyScaling(&ratioLSlider, 85, 30, 80, 80);
    setBoundsAndApplyScaling(&attackLSlider, 15, 120, 80, 80);
    setBoundsAndApplyScaling(&releaseLSlider, 85, 120, 80, 80);
    setBoundsAndApplyScaling(&kneeLSlider, 15, 210, 80, 80);
    setBoundsAndApplyScaling(&mixLSlider, 85, 210, 80, 80);
    setBoundsAndApplyScaling(&autoAttackButton, 195, 110, 70, 20);
    setBoundsAndApplyScaling(&autoReleaseButton, 285, 110, 70, 20);
    setBoundsAndApplyScaling(&autoMakeupButton, 195, 135, 70, 20);
    setBoundsAndApplyScaling(&lahButton, 285, 135, 70, 20);
    setBoundsAndApplyScaling(&powerButton, 355, 30, 30, 30);
    setBoundsAndApplyScaling(&meterbg, 175, 160, 210, 130);
    setBoundsAndApplyScaling(&meter, 175, 160, 210, 130);
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
