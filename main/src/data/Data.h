#pragma once

// #include <Arduino.h>
// #include "pinout_esp32.h"
// #include "pin_handle.h"
// #include "Peripheral.h"
// #include "Bluetooth.h"
// #include "MIDIControl.h"
// #include "Communication.h"
// #include "Multiplexor.h"
// #include "bitmap.h"
// #include "LMUtil.h"
// #include "LMThread.h"

enum class StateType : uint8_t
{
    Digital,
    Analog,
    Encoder
};

class StateBase
{
public:
    explicit StateBase(uint8_t id) : id_(id) {}
    virtual ~StateBase() = default;

    StateBase(const StateBase&) = delete;
    StateBase& operator=(const StateBase&) = delete;

    virtual StateType type() const = 0;

    uint8_t id() const { return id_; }
    bool changed() const { return changed_; }
    void clearChanged() { changed_ = false; }

protected:
    void markChanged() { changed_ = true; }

private:
    uint8_t id_;
    bool changed_ = false;
};

class ButtonState : public StateBase
{
public:
    StateType type() const override { return StateType::Digital; }

    void set(bool pressed)
    {
        pressed_ = pressed;
        markChanged();
    }

private:
bool pressed_ = false;
};