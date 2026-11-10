#include <Arduino.h>
#include "pinout_esp32.h"
#include "pin_handle.h"
#include "Peripheral.h"
#include "Bluetooth.h"
#include "MIDIControl.h"
#include "Communication.h"

MIDIController controller;
MIDIOut midiOut;

PeripheralHandle::Button myButton("button", PinAddress::GPIO_13);
PeripheralHandle::Potentiometer myPot("pot", PinAddress::GPIO_34);
PeripheralHandle::RotaryEncoder myEnc("enc", PinAddress::GPIO_18, PinAddress::GPIO_19, PinAddress::GPIO_21);

// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 32
// #define OLED_ADDR 0x3C

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
PeripheralHandle::Oled_SSD1306 myDisplay("oled");

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL

  controller.setOutput(midiOut);

  controller.addControl(myButton);
  controller.addControl(myPot);
  controller.addControl(myEnc);
  // controller.addControl(myEnc, 20); // ControlHandle, cc number

  controller.setup();

  myDisplay.setup();

  // // HUB
  // hub_setup();
  
  // MODULE
  module_setup();
  
}

void loop() {
  controller.update();
  myDisplay.update();

  // int buttonState = myButton.getValue();
  // int potValuåe = myPot.getValue();
  // int encButtonValue = myEnc.getValueButton();
  // int encPosition = myEnc.getValuePosition();

  // Serial.printf("%d %d %d %d\n", encButtonValue, encPosition, potValue, buttonState);
  controller.sendMidi();

  // // HUB
  // hub_loop();
  
  // MODULE
  module_loop();

  delay(40);
}



