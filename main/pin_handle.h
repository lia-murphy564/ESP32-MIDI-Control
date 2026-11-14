#pragma once
#include <Arduino.h>
#include "pinout_esp32.h"

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

    void setup()
    {
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

    bool hasPin() const
    {
      return hasPin_;
    }

  private:
    PinAddress pin_;
    PinMode mode_;
    bool hasPin_ = false;
};

