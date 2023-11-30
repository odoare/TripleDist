#include <JuceHeader.h>

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
  MyLookAndFeel(juce::Colour col)
  {
    setColour(col);
  }

  void drawRotarySlider(juce::Graphics &g, 
          int x, int y, 
          int width,
          int height, 
          float sliderPos, 
          float rotaryStartAngle, 
          float rotaryEndAngle, 
          juce::Slider &slider) override
  {
    float diameter = juce::jmin(width,height);
    float radius = diameter * 0.5;
    float centreX = x + width * 0.5;
    float centreY = y + height * 0.5;
    float rx = centreX - radius;
    float ry = centreY - radius;
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle-rotaryStartAngle));

    juce::Rectangle<float> dialArea(rx,ry,diameter,diameter);
    g.setColour(juce::Colours::black);
    //g.drawRect(dialArea);
    g.fillEllipse(dialArea);
    g.setColour(sliderColour);
    g.drawEllipse(dialArea,2.0f);
    // g.drawText(juce::String());

    juce::Path dialTick;
    juce::Rectangle<int> rect(0,-radius,4.0f,radius*0.33);
    // juce::Point point(centreX,centreY);
    dialTick.addRectangle(rect);
    g.fillPath(dialTick,juce::AffineTransform::rotation(angle).translated(centreX,centreY));
    // juce::DropShadow shadow(juce::Colours::white,30,point);
    // shadow.drawForRectangle(g,rect);
  };

  juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override
  {
    auto localBounds = slider.getLocalBounds().reduced(5).toFloat();
    juce::Slider::SliderLayout layout;
    layout.sliderBounds = slider.getLocalBounds().reduced(5);
    layout.textBoxBounds = slider.getLocalBounds().reduced(5).withSizeKeepingCentre (localBounds.getWidth() * 0.7f,
                                                                localBounds.getHeight() * 0.25f);
    return layout;
  }

  void drawLabel(juce::Graphics &g, juce::Label &label) override
  {
    g.setColour(juce::Colours::white);
    
    auto labelArea{label.getLocalBounds().toFloat()};
    g.setFont(labelArea.getHeight() * 0.8f);
    auto center = labelArea.getCentre();
   
    juce::String labelToDisplay = juce::String(label.getText());
    g.drawSingleLineText(labelToDisplay,
                          juce::roundToInt(center.x + g.getCurrentFont().getHorizontalScale()),
                          juce::roundToInt(center.y + g.getCurrentFont().getDescent()),
                          juce::Justification::horizontallyCentred);
    }

    void setColour(juce::Colour newCol)
    {
      sliderColour = newCol;
    }

private:
  juce::Colour sliderColour;
};
