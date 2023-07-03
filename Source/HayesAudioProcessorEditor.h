#pragma once
#include <JuceHeader.h>

#include "HayesAudioProcessor.h"

constexpr int SLIDER_COUNT = 4;

class HayesAudioProcessorEditor  : public juce::AudioProcessorEditor
                                 , public juce::Slider::Listener
{
public:
    HayesAudioProcessorEditor (HayesAudioProcessor&);
    ~HayesAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider*) override;

private:
    juce::Slider sliders[SLIDER_COUNT];
    juce::Label labels[SLIDER_COUNT];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachments[SLIDER_COUNT];
    
    HayesAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesAudioProcessorEditor)
};
