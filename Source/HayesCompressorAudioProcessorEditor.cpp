#include "HayesCompressorAudioProcessor.h"
#include "HayesCompressorAudioProcessorEditor.h"
#include "DbSlider.h"
#include "LogMsSlider.h"


HayesCompressorAudioProcessorEditor::HayesCompressorAudioProcessorEditor(HayesCompressorAudioProcessor& p)
:   AudioProcessorEditor (&p)
,   processor (p)
{
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    const char* paramNames[SLIDER_COUNT] = { "threshold", "ratio", "inputgain", "attack", "release",  "outputgain" };
    const char* labelNames[SLIDER_COUNT] = { "Threshold", "Ratio", "Input Gain", "Attack", "Release", "Output Gain" };
    
    for (int i = 0; i < SLIDER_COUNT; ++i)
    {
        labels[i].setText(labelNames[i], juce::dontSendNotification);
        labels[i].setJustificationType(juce::Justification::centred);
        labels[i].setLookAndFeel(&customLookAndFeel);
        labels[i].setFont(juce::Font("Lucida Console", 11.f, juce::Font::bold));
        addAndMakeVisible(labels[i]);
        if (i == 0 || i == 2 || i == 5)
            sliders[i] = std::make_unique<DbSlider>();
        else if (i == 1) // ratio is linear (no unit)
            sliders[i] = std::make_unique<juce::Slider>();
        else // attack and release are logarithmic
            sliders[i] = std::make_unique<LogMsSlider>();
        sliders[i]->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        sliders[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
        sliders[i]->setNumDecimalPlacesToDisplay(2);
        sliders[i]->setLookAndFeel(&customLookAndFeel);
        sliders[i]->addListener(this);
        addAndMakeVisible(sliders[i].get());
        sliderAttachments[i] = std::make_unique<Attachment>(processor.apvts, paramNames[i], *sliders[i]);
    }

    image = juce::ImageCache::getFromMemory(BinaryData::bg_file_jpg, BinaryData::bg_file_jpgSize);
    setSize (400, 300);
}

HayesCompressorAudioProcessorEditor::~HayesCompressorAudioProcessorEditor()
{
    for (int i = 0; i < SLIDER_COUNT; ++i)
        sliders[i]->removeListener(this);
}

void HayesCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.drawImage(image, 0, 0, getWidth(), getHeight(), 0, 256, 800, 600); 
}

void HayesCompressorAudioProcessorEditor::resized()
{
    labels[0].setBounds(10, 0, 100, 20);
    sliders[0]->setBounds(0, 25, 120, 120);
    labels[1].setBounds(150, 0, 100, 20);
    sliders[1]->setBounds(140, 25, 120, 120);
    labels[2].setBounds(290, 0, 100, 20);
    sliders[2]->setBounds(280, 25, 120, 120);
    labels[3].setBounds(10, 150, 100, 20);
    sliders[3]->setBounds(0, 175, 120, 120);
    labels[4].setBounds(150, 150, 100, 20);
    sliders[4]->setBounds(143, 175, 120, 120);
    labels[5].setBounds(290, 150, 100, 20);
    sliders[5]->setBounds(280, 175, 120, 120);
}

void HayesCompressorAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    if (slider == sliders[0].get())
        processor.compressor.setThreshold(slider->getValue());
    if (slider == sliders[1].get())
        processor.compressor.setRatio(slider->getValue());
    if (slider == sliders[2].get())
        processor.inputGain = slider->getValue();
    if (slider == sliders[3].get())
        processor.compressor.setAttack(slider->getValue());
    if (slider == sliders[4].get())
        processor.compressor.setRelease(slider->getValue());
    if (slider == sliders[5].get())
        processor.outputGain = slider->getValue();
}