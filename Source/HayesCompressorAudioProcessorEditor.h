#pragma once
#include <JuceHeader.h>
#include "HayesCompressorAudioProcessor.h"
#include "gui/include/Meter.h"
#include "../../Common/BaseAudioProcessorEditor.h"
#include "../../Common/CustomLookAndFeel.h"
#include "../../Common/PresetBar.h"

constexpr int NUM_SLIDERS = 8;
constexpr int NUM_BUTTONS = 5;

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

    std::unique_ptr<juce::Slider> sliders[NUM_SLIDERS]; // ingain, makeupgain, thresh, ratio, knee, attack, release, mix
    juce::Label labels[NUM_SLIDERS];
    std::unique_ptr<juce::Button> buttons[NUM_BUTTONS]; // lah, autoatk, autorel, automakeup, power

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments[NUM_SLIDERS];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments[NUM_BUTTONS];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HayesCompressorAudioProcessorEditor)
};
