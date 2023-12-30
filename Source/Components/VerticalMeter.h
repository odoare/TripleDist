/*
  ==============================================================================

    VerticalMeter.h
    Created: 29 Nov 2023 4:38:30pm
    Author:  doare

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iostream>

namespace Gui
{
  class VerticalMeter : public juce::Component, public juce::Timer
  {
  public:
    VerticalMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
    {
      startTimerHz(24);
      meterColour=juce::Colours::white;
    }

    void paint(juce::Graphics& g) override
    {
      const auto level = valueSupplier();

      auto bounds = getLocalBounds().reduced(10).toFloat();
      auto h = bounds.getHeight();
      const auto newHeight = juce::jmap(level,-70.0f, 6.0f, 0.0f, h);
      auto dh = -h+newHeight;
      auto y = bounds.getTopLeft().getY();
      auto bounds2 = bounds;
      bounds2.setTop(y-dh);
    
      g.setColour(juce::Colours::black);
      g.fillRoundedRectangle(bounds,5.0f);

      g.setGradientFill(gradient);
      g.setColour(meterColour);
      const auto scaledY = juce::jmap(level,-70.0f, 6.0f, 0.0f, static_cast<float>(bounds.getHeight()));
      g.fillRoundedRectangle(bounds2,5.0f);
    }

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