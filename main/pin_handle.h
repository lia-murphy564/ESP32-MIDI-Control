#pragma once
#include <Arduino.h>
#include "pinout_esp32.h"

enum class PinMode : uint8_t
{
  Input         = INPUT,
  Output        = OUTPUT,
  InputPullup   = INPUT_PULLUP,
  InputPulldown = INPUT_PULLDOWN,
  Default       = 0xFF, // Custom mode for PWM output
};

class PinHandle
{
  public:
    PinHandle() {}
    explicit PinHandle(PinAddress pin) : pin_(pin), hasPin_(true) {}
    explicit PinHandle(PinAddress pin, PinMode mode) : pin_(pin), mode_(mode), hasPin_(true) {}

    void setPin(PinAddress pin)
    {
      pin_ = pin;
      hasPin_ = true;
    }

    void setMode(PinMode mode)
    {
      mode_ = mode;
    }

    PinMode getMode()
    {
      return mode_;
    }

    void setup()
    {
      if (hasPin_)
      pinMode(static_cast<uint8_t>(pin_), static_cast<uint8_t>(mode_));
    }

    int readDigital()
    {
      if (hasPin_ == false)
        return -1;
      return digitalRead(static_cast<uint8_t>(pin_));

    }

    int readAnalog()
    {
      if (hasPin_ == false)
        return -1;
      return analogRead(static_cast<uint8_t>(pin_));
    }

    void writeDigital(bool value)
    {
      if (hasPin_ == false)
        return;
      digitalWrite(static_cast<uint8_t>(pin_), value ? HIGH : LOW);
    }

    void writePWM(uint8_t value)
    {
      if (hasPin_ == false)
        return;
      analogWrite(static_cast<uint8_t>(pin_), value);
    }

    bool hasPin() const
    {
      return hasPin_;
    }

  private:
    PinAddress pin_;
    PinMode mode_ = PinMode::Default;
    bool hasPin_ = false;
};

