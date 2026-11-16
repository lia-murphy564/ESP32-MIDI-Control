#pragma once
#include <Arduino.h>
#include "pinout_esp32.h"
#include "MIDIOut.h"
#include <algorithm>
#include <Wire.h>
#include "Display.h"
#include "LMUtil.h"

namespace Peripheral
{
  struct DataState {};

  struct ButtonState : DataState
  {
    uint8_t value = 0;
  };

  struct PotentiometerState : DataState
  {
    uint8_t value = 0; // 0 - 127
  };

using PeripheralID = const char *;
  
class PeripheralBase
{
public:
  virtual ~PeripheralBase() {}

  explicit PeripheralBase(PeripheralID id = nullptr) : id_(id) {}

  virtual void setup() = 0;
  virtual void update() = 0;
  virtual PeripheralID id() const
  {
    return id_;
  }

  virtual void send(MIDIOut &midiOut) {}

private:
  PeripheralID id_;
};

class Button : public PeripheralBase
{
public:
  explicit Button(PeripheralID id, PinAddress pin) : PeripheralBase(id), pin_(pin, PinMode::InputPullup) {}
  explicit Button(PeripheralID id) : PeripheralBase(id) {}

  void setup() override
  {
    pin_.setup();
  }

  void update() override
  {
    // TODO: debounce

    if (!pin_.hasPin()) return;
    updateInternalData(pin_.readDigital());
  }

  bool changed()
  {
    if (value_ == lastSent_) return false;
    lastSent_ = value_;
    return true;
  }

  void updateInternalData(auto val)
  {
    if (!debouncer_.debounce(val == LOW)) return;

    if (true)
      value_ = !debouncer_.stable;
    else if (!debouncer_.stable)
      value_ = !value_; 
  }

  int getValue() const
  {
    return value_;
  }

  void send(MIDIOut &midiOut) override
  {
    int midiValue = (value_ == LOW) ? 127 : 0;

    if (midiValue == lastSent_) return;
    lastSent_ = midiValue;

    midiOut.controlChange(1, 20, midiValue);
  }
  
private:
  PinHandle pin_;
  int value_ = LOW;
  int lastSent_ = -1;
  LMUtil::Debouncer debouncer_;
};

class Potentiometer : public PeripheralBase
{
public:
  explicit Potentiometer(PeripheralID id, PinAddress pin) : PeripheralBase(id), pin_(pin, PinMode::Input) {}

  void setup() override
  {
    pin_.setup();
  }

  void update() override
  {
    // TODO: smoothing
    value_ = pin_.readAnalog();
  }

  bool changed()
  {
    if (value_ == lastSent_) return false;
    lastSent_ = value_;
    return true;
  }

  int getValue() const
  {
    return value_;
  }

  float getValueSmoothed() const
  {
    return smoothed_;
  }

  void send(MIDIOut &midiOut) override
  {
    int midiValue = std::clamp(value_ >> 5, 0, 127);

    if (midiValue == lastSent_) return;
    lastSent_ = midiValue;

    // midiOut.controlChange(1, 21, midiValue);
  }

private:
  PinHandle pin_;
  int value_ = 0;
  float smoothed_ = 0.0f;
  int lastSent_ = -1;
};

class RotaryEncoder : public PeripheralBase
{
public:
  explicit RotaryEncoder(PeripheralID id, PinAddress pin_a, PinAddress pin_b, PinAddress pin_sw)
    : PeripheralBase(id),
      pin_a_(pin_a, PinMode::Input),
      pin_b_(pin_b, PinMode::Input),
      button_(id, pin_sw) {}

      void setup() override
      {
        pin_a_.setup();
        pin_b_.setup();
        button_.setup();

        lastStateClock = pin_a_.readDigital();
      }

      void update() override
      {
        // pin_a_.update();
        // pin_b_.update();
        button_.update();

        currentStateClock = pin_a_.readDigital();

        if (currentStateClock != lastStateClock && currentStateClock == LOW)
        {
          if (pin_b_.readDigital() != currentStateClock)
          {
            counter++;
          }
          else
          {
            counter--;
          }
        }

        lastStateClock = currentStateClock;
      }

      int getValueButton() const
      {
        return button_.getValue();
      }

      int getValuePosition() const
      {
        return counter;
      }

      void send(MIDIOut &midiOut) override
      {
        return;
      }

private:
  PinHandle pin_a_;
  PinHandle pin_b_;
  Button button_;

  int counter;
  int currentStateClock;
  int lastStateClock;
};

//Todo: U8g2 font
class Oled_SSD1306 : public PeripheralBase
{
public:
  explicit Oled_SSD1306(PeripheralID id, TwoWire &bus = Wire, uint8_t address = 0x3C)
    : PeripheralBase(id),
      bus_(bus),
      address_(address),
      display_(width_, height_, &bus_, resetPin_) {}

  void setup() override
  {
    display_.begin(SSD1306_SWITCHCAPVCC, address_);
    display_.clearDisplay();
    display_.setTextColor(SSD1306_WHITE);

    display_.setTextSize(1);
    display_.setCursor(0, 0);
    display_.println("L1A - DEBUG");

    display_.setTextSize(2);
    display_.setCursor(0, 12);
    display_.println("CC NaN");

    display_.display();
  }

  void update() override
  {
    if (!stale_)
      return;

    updateDisplay();
    stale_ = false;
  }

  void setDisplay(DisplayData &data)
  {
    data_ = &data;
    stale_ = true;
    
    // if (val.get() == data_.get())
    //   return;
    // data_ = val;
    // stale_ = true;
  }

private:
  void updateDisplay()
  {
    display_.clearDisplay();

    if (data_)
      data_->draw(display_);

    display_.display();

  }

  TwoWire &bus_;
  uint8_t address_;
  Adafruit_SSD1306 display_;

  static constexpr uint8_t width_ = 128;
  static constexpr uint8_t height_ = 32;
  static constexpr int8_t resetPin_ = -1;

  DisplayData *data_ = nullptr;
  bool stale_ = false;
};

class LED : public PeripheralBase
{
public:
  explicit LED(PeripheralID id, PinAddress pin)
    : PeripheralBase(id),
      pin_(pin, PinMode::Output) {
        isPWM_ = false;
      }

  explicit LED(PeripheralID id, PinAddress pin, bool usePWM)
    : PeripheralBase(id),
      pin_(pin, usePWM ? PinMode::Default : PinMode::Output) {
        if (pin_.getMode() == PinMode::Default)
          isPWM_ = true;
      }

    void setup() override
    {
      pin_.setup();
    }

    void update() override
    {
      if (isBlinking)
      {
        if (millis() - lastBlinkTime_ >= blinkPeriod_)
        {
          if (isPWM_)
          {
            pwmValue_ += fadeDirection_;
            if (pwmValue_ == 0 || pwmValue_ >= 255)
              fadeDirection_ = -fadeDirection_;
          }
          else
          {
            state_ = !state_;
            if (state_ > 1)
              state_ = 1;
          }

          lastBlinkTime_ = millis();
        }
      }

      if (isPWM_)
        pin_.writePWM(state_ ? LMUtil::gamma8(pwmValue_) : 0);
      else
        pin_.writeDigital(state_);
    }

    void set(uint8_t state)
    {
      state_ = state;
      update();
    }

    void toggle()
    {
      state_ = !state_;
    }

    void setBlink(uint16_t period)
    {
      blinkPeriod_ = period;

      if (blinkPeriod_ == 0)
      {
        isBlinking = false;
        return;
      }

      isBlinking = true;
      lastBlinkTime_ = millis();
    }

    void pwm(uint8_t value)
    {
      pwmValue_ = value;
      update();
    }

private: 
  PinHandle pin_;

  bool isBlinking = false;

  // Digital
  uint8_t state_ = 0;
  uint16_t blinkPeriod_ = 0;
  uint32_t lastBlinkTime_ = 0;

  // PWM
  bool isPWM_ = false;
  uint8_t pwmValue_ = 0; // 0 - 255
  int8_t fadeDirection_ = 1; // 1 = up, -1 = down
};

} // namespace Peripheral

