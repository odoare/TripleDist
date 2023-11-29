/*
  ==============================================================================

    VerticalGradientMeter.h
    Created: 29 Nov 2023 4:38:30pm
    Author:  doare

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Gui
{
  class VerticalGradientMeter : public juce::Component
  {
  public:
    VerticalGradientMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
    {


    }

    void paint(juce::Graphics& g) override
    {
        const auto level = valueSupplier();

        auto bounds = getLocalBounds().toFloat();

        g.setColour(juce::Colours::black);
        g.fillRect(bounds);

        g.setGradientFill(gradient);
        

    }

    void resized() override
    {
      const auto bounds = getLocalBounds().toFloat();
      juce::ColourGradient gradient{
        juce::Colours::green,
        bounds.getBottomLeft(),
        juce::Colours::red,
        bounds.getTopLeft(),
        false
      };

      gradient.addColour(0.5,juce::Colours::yellow);

    }

  private:
    std::function<float()> valueSupplier;

  }
}