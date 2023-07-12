#pragma once
#include <JuceHeader.h>

#include "HayesCompressorAudioProcessor.h"
#include "CustomLookAndFeel.h"

constexpr int SLIDER_COUNT = 6;

class HayesCompressorAudioProcessorEditor  : public juce::AudioProcessorEditor
                                 , public juce::Slider::Listener
{
public:
    HayesCompressorAudioProcessorEditor (HayesCompressorAudioProcessor&);
    ~HayesCompressorAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider*) override;

private:
    HayesCompressorAudioProcessor& processor;
    
    CustomLookAndFeel customLookAndFeel;
    
    juce::Image image;

    std::unique_ptr<juce::Slider> sliders[SLIDER_COUNT];
    juce::Label labels[SLIDER_COUNT];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments[SLIDER_COUNT];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesCompressorAudioProcessorEditor)
};
