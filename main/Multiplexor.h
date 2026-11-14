#pragma once

#include "pinout_esp32.h"
#include "Peripheral.h"

namespace PeripheralHandle
{
    class Multiplexor : public PeripheralBase
    {
    public:
        Multiplexor(PeripheralID id, PinAddress s0, PinAddress s1, PinAddress s2, PinAddress s3, PinAddress sig)
            : PeripheralBase(id),
              s0_(s0, PinMode::Output),
              s1_(s1, PinMode::Output),
              s2_(s2, PinMode::Output),
              s3_(s3, PinMode::Output),
              sig_(sig, PinMode::InputPullup)
        {}

        void setup()
        {
            s0_.setup();
            s1_.setup();
            s2_.setup();
            s3_.setup();
            sig_.setup();
        }

        void update() override
        {
            for (uint8_t ch = 0; ch < 16; ch++)
            {
                if (buttons_[ch] == nullptr) continue;
                buttons_[ch]->updateInternalData(readChannel(ch));
            }
            // state_ = readAll();
        }

        void attach(Button &button, uint8_t ch)
        {
            if (ch > 0x0F) return;
            buttons_[ch] = &button;
        }

        uint8_t readChannel(uint8_t ch)
        {
            setChannel(ch);
            delayMicroseconds(5); // sig delay
            return sig_.readDigital();
        }

        uint16_t readAll()
        {
            uint16_t state = 0;
            for (uint8_t ch = 0; ch < 16; ch++)
            {
                if (readChannel(ch))
                    state |= (1 << ch);
            }
            return state;
        }

    private:
        void setChannel(uint8_t ch)
        {
            ch &= 0x0F;
            if (ch == currentCh_) return;

            s0_.writeDigital(ch & 0x01);
            s1_.writeDigital((ch >> 1) & 0x01);
            s2_.writeDigital((ch >> 2) & 0x01);
            s3_.writeDigital((ch >> 3) & 0x01);
            
            currentCh_ = ch;
        }

        PinHandle s0_;
        PinHandle s1_;
        PinHandle s2_;
        PinHandle s3_;
        PinHandle sig_;
        uint8_t currentCh_ = 0xFF; // invalid byte

        uint16_t state_ = 0;

        Button* buttons_[16] = {};
    };
}