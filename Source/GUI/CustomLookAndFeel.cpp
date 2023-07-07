#include "CustomLookAndFeel.h"
#include "CustomColours.h"

EditorLnf::EditorLnf()
{
}

void EditorLnf::drawCornerResizer (juce::Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging)
{
    juce::ignoreUnused (isMouseDragging);

    auto lineThickness = juce::jmin ((float) w, (float) h) * 0.07f;

    for (float i = 0.0f; i < 1.0f; i += 0.3f)
    {
        auto colour = isMouseOver ? CustomColours::blue : CustomColours::blackGrey;
        g.setColour (colour);

        g.drawLine ((float) w * i,
                    (float) h + 1.0f,
                    (float) w + 1.0f,
                    (float) h * i,
                    lineThickness);

        g.drawLine ((float) w * i + lineThickness,
                    (float) h + 1.0f,
                    (float) w + 1.0f,
                    (float) h * i + lineThickness,
                    lineThickness);
    }
}
