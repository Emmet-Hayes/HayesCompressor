#pragma once
#include <JuceHeader.h>
#include "HayesCompressorAudioProcessor.h"
#include "gui/include/LabeledSlider.h"
#include "gui/include/Meter.h"
#include "CustomLookAndFeel.h"
#include "PresetBar.h"

class HayesCompressorAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Button::Listener, juce::Timer
{
public:
    HayesCompressorAudioProcessorEditor(HayesCompressorAudioProcessor&);
    ~HayesCompressorAudioProcessorEditor();

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button*) override;
    void timerCallback() override;

private:
    void initWidgets();
    void setGUIState(bool);

    HayesCompressorAudioProcessor& processor;

    CustomLookAndFeel customLookAndFeel;

    juce::Image image;

    //Widgets
    PresetBar presetBar;
    MeterBackground meterbg;
    Meter meter;

    LabeledSlider inGainLSlider;
    LabeledSlider makeupGainLSlider;
    LabeledSlider treshLSlider;
    LabeledSlider ratioLSlider;
    LabeledSlider kneeLSlider;
    LabeledSlider attackLSlider;
    LabeledSlider releaseLSlider;
    LabeledSlider mixLSlider;

    juce::TextButton lahButton;
    juce::TextButton autoAttackButton;
    juce::TextButton autoReleaseButton;
    juce::TextButton autoMakeupButton;
    juce::DrawableButton powerButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lahAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoMakeupAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> powerAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HayesCompressorAudioProcessorEditor)
};
