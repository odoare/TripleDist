/*
  ==============================================================================

    VerticalGradientMeter.h
    Created: 29 Nov 2023 4:38:30pm
    Author:  doare

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iostream>

namespace Gui
{
  class VerticalGradientMeter : public juce::Component, public juce::Timer
  {
  public:
    VerticalGradientMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
    {
      startTimerHz(24);
      meterColour=juce::Colours::white;
    }

    void paint(juce::Graphics& g) override
    {
        const auto level = valueSupplier();

        auto bounds = getLocalBounds().reduced(10).toFloat();
        auto h = bounds.getHeight();
        const auto newHeight = juce::jmap(level,-30.0f, 6.0f, 0.0f, h);
        auto dh = -h+newHeight;
        auto y = bounds.getTopLeft().getY();
        auto bounds2 = bounds;
        bounds2.setTop(y-dh);
      
        g.setColour(juce::Colours::black);
        g.fillRoundedRectangle(bounds,5.0f);

        g.setGradientFill(gradient);
        g.setColour(meterColour);
        const auto scaledY = juce::jmap(level,-30.0f, 6.0f, 0.0f, static_cast<float>(bounds.getHeight()));
        // std::cout << scaledY << "\n";
        g.fillRoundedRectangle(bounds2,5.0f);
    }

    // void resized() override
    // {
    //   auto bounds = getLocalBounds().toFloat();
    //   juce::ColourGradient gradient{juce::Colours::green, bounds.getBottomLeft(), juce::Colours::red, bounds.getTopLeft(), false};
    //   gradient.addColour(0.5,juce::Colours::yellow);
    // }

    void timerCallback() override
    {
      repaint();
    }

    void setColour(juce::Colour newColour)
    {
      meterColour = newColour;
    }

  private:
    std::function<float()> valueSupplier;
    juce::ColourGradient gradient{};
    juce::Colour meterColour;

  };
}
