#pragma once

#include "JuceHeader.h"

#include "components/look_and_feel/colours.hpp"
#include "components/look_and_feel/fonts.hpp"
#include "components/ear_button.hpp"
#include "components/ear_slider.hpp"

namespace ear {
namespace plugin {
namespace ui {

    class TooltipLookAndFeel : public LookAndFeel_V4
    {
    public:
      TooltipLookAndFeel()
      {
        setColour(TooltipWindow::backgroundColourId, EarColours::Primary.darker());
        setColour(TooltipWindow::textColourId, juce::Colours::white);
      }

      TextLayout layoutTooltipText (const String& text, Colour colour) noexcept
      {
        const float tooltipFontSize = 13.0f;
        const int maxToolTipWidth = 400;

        AttributedString s;
        s.setJustification (Justification::centred);
        s.append (text, Font (tooltipFontSize, Font::plain), colour);

        TextLayout tl;
        tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
        return tl;
      }

      void drawTooltip (Graphics& g, const String& text, int width, int height) override
      {
        Rectangle<int> bounds (width, height);
        auto cornerSize = 5.0f;

        g.setColour (findColour (TooltipWindow::backgroundColourId));
        g.fillRoundedRectangle (bounds.toFloat(), cornerSize);

        g.setColour (findColour (TooltipWindow::outlineColourId));
        g.drawRoundedRectangle (bounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);

        layoutTooltipText (text, findColour (TooltipWindow::textColourId))
          .draw (g, { static_cast<float> (width), static_cast<float> (height) });
      }

    };


class ValueBoxOsc : public Component {
public:
   ValueBoxOsc() :
        enableButton_(std::make_shared<EarButton>()),
        portLabel_(std::make_unique<Label>()),
        portControl_(std::make_shared<EarSlider>()),
        statusLabel_(std::make_shared<Label>()) {
    setColour(backgroundColourId, EarColours::Area01dp);

    enableButton_->setButtonText("Enable OSC");
    enableButton_->setShape(EarButton::Shape::Toggle);
    // Note: Do NOT enableButton_->setClickingTogglesState(true);
    /// It is false by default and State is handled completely by
    ///  BinauralMonitoringJuceFrontendConnector
    enableButton_->setEnabled(true);
    enableButton_->setTooltip ("Note: To minimise head-tracking latency, turn off any anticipative processing in the DAW.");
    addAndMakeVisible(enableButton_.get());

    tooltipWindow.setLookAndFeel(&tooltipLookAndFeel);
    tooltipWindow.setOpaque(false);

    portLabel_->setFont(EarFonts::Values); //(EarFonts::Label);
    portLabel_->setColour(Label::textColourId, EarColours::Label);
    portLabel_->setText("Port", juce::NotificationType::dontSendNotification);
    portLabel_->setJustificationType(Justification::left);
    addAndMakeVisible(portLabel_.get());

    portControl_->setSliderStyle(Slider::IncDecButtons);
    portControl_->setTextBoxStyle(Slider::TextBoxRight, false, 44, 40);
    portControl_->setRange(1, 65535, 1);
    portControl_->setNumDecimalPlacesToDisplay(0);
    portControl_->setDoubleClickReturnValue(true, 8000);
    portControl_->setMouseDragSensitivity(65535);
    portControl_->setIncDecButtonsMode(juce::Slider::IncDecButtonMode::incDecButtonsNotDraggable);
    addAndMakeVisible(portControl_.get());

    statusLabel_->setFont(EarFonts::Units);
    statusLabel_->setColour(Label::textColourId, EarColours::Label);
    statusLabel_->setText("Initialising...", juce::NotificationType::dontSendNotification);
    statusLabel_->setJustificationType(Justification::left);
    addAndMakeVisible(statusLabel_.get());
  }

  ~ValueBoxOsc() {}

  void paint(Graphics& g) override {
    // background
    g.fillAll(findColour(backgroundColourId));
  }

  enum ColourIds {
    backgroundColourId = 0x00010001,
  };

  void resized() override {
    auto area = getLocalBounds();
    float yReduction = ((float)area.getHeight() - narrowRowHeight_) / 2.f;
    area.reduce(marginSmall_, yReduction);

    area.removeFromLeft(marginBig_);
    enableButton_->setBounds(area.removeFromLeft(140));
    portLabel_->setBounds(area.removeFromLeft(35));
    portControl_->setBounds(area.removeFromLeft(50));
    area.removeFromLeft(marginBig_ + marginBig_);
    statusLabel_->setBounds(area);
  }

  std::shared_ptr<ear::plugin::ui::EarButton> getEnableButton() { return enableButton_; }
  std::shared_ptr<EarSlider> getPortControl() { return portControl_; }
  std::shared_ptr<Label> getStatusLabel() { return statusLabel_; }

private:
  std::unique_ptr<Label> portLabel_;
  std::shared_ptr<ear::plugin::ui::EarButton> enableButton_;
  std::shared_ptr<EarSlider> portControl_;
  std::shared_ptr<Label> statusLabel_;
  TooltipWindow tooltipWindow{ this };
  TooltipLookAndFeel tooltipLookAndFeel;

  const float marginSmall_ = 5.f;
  const float marginBig_ = 10.f;
  const float narrowRowHeight_ = 24.f;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueBoxOsc)
};

}  // namespace ui
}  // namespace plugin
}  // namespace ear
