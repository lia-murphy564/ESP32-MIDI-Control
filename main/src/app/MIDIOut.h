#pragma once
#include <Arduino.h>
// #include "MIDIUSB.h" 
#include <mutex> 
#include <vector>

//Todo: add port selection, for now its just serial
//Todo: add ble, usbmidi, serial select
class MIDIOut
{
public:
  MIDIOut() {};
  virtual ~MIDIOut() {};

  void controlChange(uint8_t channel, uint8_t cc, uint8_t value)
  {
    send(0xB0 | ((channel - 1) & 0x0F), cc & 0x7F, value & 0x7F);
  }

  void noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
  {
    send(0x90 | (channel & 0x0F), note & 0x7F, velocity & 0x7F);
  }

  void noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
  {
    send(0x80 | (channel & 0x0F), note & 0x7F, velocity & 0x7F);
  }

private:
  void send(uint8_t status, uint8_t data1, uint8_t data2)
  {
    uint8_t msg[3] = {status, data1, data2};
    Serial.write(msg, 3);
  }
};