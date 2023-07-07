#include "HayesAudioProcessor.h"
#include "HayesAudioProcessorEditor.h"
#include "DbSlider.h"
#include "LogMsSlider.h"


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
        if (i < 1)
            sliders[i] = std::make_unique<DbSlider>();
        else if (i < 2) // ratio is linear (no unit)
            sliders[i] = std::make_unique<juce::Slider>();
        else // attack and release are logarithmic
            sliders[i] = std::make_unique<LogMsSlider>();
        sliders[i]->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
        sliders[i]->setNumDecimalPlacesToDisplay(2);
        sliders[i]->addListener(this);
        addAndMakeVisible(sliders[i].get());
        sliderAttachments[i] = std::make_unique<Attachment>(processor.apvts, paramNames[i], *sliders[i]);
    }

    image = juce::ImageCache::getFromMemory(BinaryData::bg_file_jpg, BinaryData::bg_file_jpgSize);
    setSize (400, 300);
}

HayesAudioProcessorEditor::~HayesAudioProcessorEditor()
{
    for (int i = 0; i < SLIDER_COUNT; ++i)
        sliders[i]->removeListener(this);
}

void HayesAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 256, 800, 600); 
}

void HayesAudioProcessorEditor::resized()
{
    labels[0].setBounds(10, 0, 100, 20);
    sliders[0]->setBounds(0, 25, 120, 120);
    labels[1].setBounds(280, 0, 100, 20);
    sliders[1]->setBounds(270, 25, 120, 120);
    labels[2].setBounds(5, 150, 100, 20);
    sliders[2]->setBounds(0, 175, 120, 120);
    labels[3].setBounds(280, 150, 100, 20);
    sliders[3]->setBounds(270, 175, 120, 120);
}

void HayesAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == sliders[0].get())
        processor.compressor.setThreshold(slider->getValue());
    if (slider == sliders[1].get())
        processor.compressor.setRatio(slider->getValue());
    if (slider == sliders[2].get())
        processor.compressor.setAttack(slider->getValue());
    if (slider == sliders[3].get())
        processor.compressor.setRelease(slider->getValue());
}