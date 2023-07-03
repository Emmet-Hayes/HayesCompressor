#include "HayesAudioProcessor.h"
#include "HayesAudioProcessorEditor.h"

HayesAudioProcessorEditor::HayesAudioProcessorEditor (HayesAudioProcessor& p)
:   AudioProcessorEditor (&p)
,   processor (p)
{
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    const char* paramNames[SLIDER_COUNT] = { "threshold", "ratio", "attack", "release" };
    const char* labelNames[SLIDER_COUNT] = { "Threshold", "Ratio", "Attack", "Release" };
    
    for (int i = 0; i < SLIDER_COUNT; ++i)
    {
        labels[i].setText(labelNames[i], juce::dontSendNotification);
        labels[i].setJustificationType(juce::Justification::centred);
        addAndMakeVisible(labels[i]);
        sliders[i].addListener(this);
        addAndMakeVisible(sliders[i]);
        sliderAttachments[i] = std::make_unique<Attachment>(processor.apvts, paramNames[i], sliders[i]);
    }

    setSize (400, 300);
}

HayesAudioProcessorEditor::~HayesAudioProcessorEditor()
{
    for (int i = 0; i < SLIDER_COUNT; ++i)
        sliders[i].removeListener(this);
}

void HayesAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void HayesAudioProcessorEditor::resized()
{
    labels[0].setBounds(0, 0, 100, 20);
    sliders[0].setBounds(0, 30, 200, 150);
    labels[1].setBounds(210, 0, 100, 20);
    sliders[1].setBounds(210, 30, 200, 150);
    labels[2].setBounds(0, 160, 100, 20);
    sliders[2].setBounds(0, 190, 200, 150);
    labels[3].setBounds(210, 160, 100, 20);
    sliders[3].setBounds(210, 190, 200, 150);
}

void HayesAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &sliders[0])
        processor.compressor.setThreshold(slider->getValue());
    if (slider == &sliders[1])
        processor.compressor.setRatio(slider->getValue());
    if (slider == &sliders[2])
        processor.compressor.setAttack(slider->getValue());
    if (slider == &sliders[3])
        processor.compressor.setRelease(slider->getValue());
}