#pragma once
#include <JuceHeader.h>
#include "HayesCompressorAudioProcessor.h"
#include "gui/include/Meter.h"
#include "../../Common/LabeledSlider.h"
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
    void addAllGUIComponents();
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

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lahAttachment, autoAttackAttachment,
                                                                          autoReleaseAttachment,
                                                                          autoMakeupAttachment, powerAttachment;

    static constexpr int defaultWidth  { 400 };
    static constexpr int defaultHeight { 300 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HayesCompressorAudioProcessorEditor)
};
