#pragma once
#include <Arduino.h>
#include "pinout_esp32.h"

class ControlBase
{
public:
  virtual ~ControlBase() {}

  virtual void setup() = 0;
  virtual void update() = 0;
};

class Button : public ControlBase
{
public:
  explicit Button(PinAddress pin) : pin_(pin, PinMode::InputPullup) {}

  void setup() override
  {
    pin_.setup();
  }

  void update() override
  {
    // TODO: debounce
    value_ = pin_.readDigital();
  }

  int getValue() const
  {
    return value_;
  }

private:
  PinHandle pin_;
  int value_ = LOW;
};

class Potentiometer : public ControlBase
{
public:
  explicit Potentiometer(PinAddress pin) : pin_(pin, PinMode::Input) {}

  void setup() override
  {
    pin_.setup();
  }

  void update() override
  {
    // TODO: smoothing
    value_ = pin_.readAnalog();
  }

  int getValue() const
  {
    return value_;
  }

private:
  PinHandle pin_;
  int value_ = 0;
};

class RotaryEncoder : public ControlBase
{
public:
  explicit RotaryEncoder(PinAddress pin_a, PinAddress pin_b, PinAddress pin_sw)
    : pin_a_(pin_a, PinMode::InputPullup),
      pin_b_(pin_b, PinMode::InputPullup),
      button_(pin_sw) {}

      void setup() override
      {
        pin_a_.setup();
        pin_b_.setup();
        button_.setup();
      }

      void update() override
      {
        button_.update();
      }

      int getValueButton() const
      {
        return button_.getValue();
      }

private:
  PinHandle pin_a_;
  PinHandle pin_b_;
  Button button_;
};

