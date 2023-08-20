
#include "HayesCompressorAudioProcessor.h"
#include "HayesCompressorAudioProcessorEditor.h"
#include "util/include/Constants.h"
#include "../../Common/DbSlider.h"
#include "../../Common/LogMsSlider.h"
#include "../../Common/PercentSlider.h"


HayesCompressorAudioProcessorEditor::HayesCompressorAudioProcessorEditor(HayesCompressorAudioProcessor& p)
:   BaseAudioProcessorEditor(p)
,   processor(p)
,   presetBar(p)
{
    setLookAndFeel(&customLookAndFeel);
    initWidgets();
    setSize(400, 300);
    startTimerHz(60);
    image = juce::ImageCache::getFromMemory(BinaryData::bg_file_jpg, BinaryData::bg_file_jpgSize);
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
    presetBar.setBounds(0, 0, 400, 20);
    sliders[0]->setBounds(155, 55, 70, 70);
    sliders[1]->setBounds(225, 55, 70, 70);
    sliders[2]->setBounds(15, 45, 60, 60);
    sliders[3]->setBounds(15, 135, 60, 60);
    sliders[4]->setBounds(15, 225, 60, 60);
    sliders[5]->setBounds(85, 45, 60, 60);
    sliders[6]->setBounds(85, 135, 60, 60);
    sliders[7]->setBounds(85, 225, 60, 60);
    meterbg.setBounds(155, 155, 230, 130);
    meter.setBounds(155, 155, 230, 130);
    buttons[0]->setBounds(315, 55, 70, 20);
    buttons[1]->setBounds(315, 80, 70, 20);
    buttons[2]->setBounds(315, 105, 70, 20);
    buttons[3]->setBounds(315, 130, 70, 20);
    buttons[4]->setBounds(375, 25, 20, 20);
}


void HayesCompressorAudioProcessorEditor::buttonClicked(juce::Button* b)
{
    if (b == buttons[1].get()) sliders[5]->setEnabled(!sliders[5]->isEnabled());
    if (b == buttons[2].get()) sliders[6]->setEnabled(!sliders[6]->isEnabled());
    if (b == buttons[4].get()) setGUIState(buttons[4]->getToggleState());
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
    auto initialise_slider = [&](juce::Slider* slider, juce::Label* label, 
                                 std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment, 
                                 const char* name, const char* paramName)
    {
        slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
        slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
        label->setText(name, juce::dontSendNotification);
        label->attachToComponent(slider, false);
        addAndMakeVisible(slider);
        attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, paramName, *slider));
    };

    sliders[0] = std::make_unique<DbSlider>();
    initialise_slider(sliders[0].get(), &labels[0], std::move(sliderAttachments[0]), "Input",     "inputgain");
    sliders[1] = std::make_unique<DbSlider>();
    initialise_slider(sliders[1].get(), &labels[1], std::move(sliderAttachments[1]), "Makeup",    "makeup");
    sliders[2] = std::make_unique<DbSlider>();
    initialise_slider(sliders[2].get(), &labels[2], std::move(sliderAttachments[2]), "Threshold", "threshold");
    sliders[3] = std::make_unique<juce::Slider>();
    initialise_slider(sliders[3].get(), &labels[3], std::move(sliderAttachments[3]), "Ratio",     "ratio");
    sliders[4] = std::make_unique<DbSlider>();
    initialise_slider(sliders[4].get(), &labels[4], std::move(sliderAttachments[4]), "Knee",      "knee");
    sliders[5] = std::make_unique<LogMsSlider>();
    initialise_slider(sliders[5].get(), &labels[5], std::move(sliderAttachments[5]), "Attack",    "attack");
    sliders[6] = std::make_unique<LogMsSlider>();
    initialise_slider(sliders[6].get(), &labels[6], std::move(sliderAttachments[6]), "Release",   "release");
    sliders[7] = std::make_unique<PercentSlider>();
    initialise_slider(sliders[7].get(), &labels[7], std::move(sliderAttachments[7]), "Mix",       "mix");

    auto initialise_button = [&](juce::Button* button, 
                                 std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment,
                                 const char* buttonText, const char* paramName, bool shouldAddAsListener)
    {
        addAndMakeVisible(button);
        button->setButtonText(buttonText);
        button->setClickingTogglesState(true);
        button->setToggleState(false, juce::dontSendNotification);
        if (shouldAddAsListener)
            button->addListener(this);
        attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, paramName, *button));
    };

    buttons[0] = std::make_unique<juce::TextButton>();
    initialise_button(buttons[0].get(), std::move(buttonAttachments[0]), "LookAhead",  "lookahead", false);
    buttons[1] = std::make_unique<juce::TextButton>();
    initialise_button(buttons[1].get(), std::move(buttonAttachments[1]), "AutoAttack",  "autoattack", true);
    buttons[2] = std::make_unique<juce::TextButton>();
    initialise_button(buttons[2].get(), std::move(buttonAttachments[2]), "AutoRelease", "autorelease", true);
    buttons[3] = std::make_unique<juce::TextButton>();
    initialise_button(buttons[3].get(), std::move(buttonAttachments[3]), "AutoMakeup",  "automakeup", true);

    buttons[4] = std::make_unique<juce::DrawableButton>("powerButton", juce::DrawableButton::ButtonStyle::ImageOnButtonBackground);
    initialise_button(buttons[4].get(), std::move(buttonAttachments[4]), "Power", "power", true);
    dynamic_cast<juce::DrawableButton*>(buttons[4].get())->setImages(
        juce::Drawable::createFromImageData(BinaryData::power_white_svg, BinaryData::power_white_svgSize).get());

    addAndMakeVisible(meter);
    meter.setMode(Meter::Mode::GR);
    
    addAndMakeVisible(presetBar);
}

void HayesCompressorAudioProcessorEditor::setGUIState(bool powerState)
{
    for (int i = 0; i < NUM_SLIDERS; ++i)
        sliders[i]->setEnabled(powerState);
 
    meter.setEnabled(powerState);
    meter.setGUIEnabled(powerState);

    for (int i = 0; i < NUM_BUTTONS - 1; ++i)
        buttons[i]->setEnabled(powerState);

    if (!powerState)
    {
        sliders[5]->setEnabled(powerState);
        sliders[6]->setEnabled(powerState);
    }
    else
    {
        sliders[5]->setEnabled(!buttons[1]->getToggleState());
        sliders[6]->setEnabled(!buttons[2]->getToggleState());
    }
}
