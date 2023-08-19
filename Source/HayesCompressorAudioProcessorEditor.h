#pragma once
#include <JuceHeader.h>
#include "HayesCompressorAudioProcessor.h"
#include "gui/include/LabeledSlider.h"
#include "gui/include/Meter.h"
#include "../../Common/BaseAudioProcessorEditor.h"
#include "../../Common/CustomLookAndFeel.h"
#include "../../Common/PresetBar.h"

class HayesCompressorAudioProcessorEditor : public BaseAudioProcessorEditor
                                          , public juce::Button::Listener
                                          , juce::Timer
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

    CustomLookAndFeel customLookAndFeel;
    
    HayesCompressorAudioProcessor& processor;
    
    juce::Image image;

    PresetBar presetBar;
    
    MeterBackground meterbg;
    Meter meter;
    LabeledSlider inGainLSlider, makeupGainLSlider, treshLSlider, ratioLSlider,
                  kneeLSlider, attackLSlider, releaseLSlider, mixLSlider;
    juce::TextButton lahButton, autoAttackButton, autoReleaseButton, autoMakeupButton;
    juce::DrawableButton powerButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lahAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoReleaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoMakeupAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> powerAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HayesCompressorAudioProcessorEditor)
};
