#pragma once

#include <JuceHeader.h>

class EditorLnf : public juce::LookAndFeel_V4
{
public:
    EditorLnf();

    void drawCornerResizer (juce::Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging) override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorLnf)
};
