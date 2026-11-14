#include <Arduino.h>
#include "pinout_esp32.h"
#include "pin_handle.h"
#include "Peripheral.h"
#include "Bluetooth.h"
#include "MIDIControl.h"
#include "Communication.h"
#include "Multiplexor.h"
#include "bitmap.h"

// NOTE: implementation is inside .h files for easy prototype development. I know i knowwww but this compiles and works. Todo: Move implementation to .cpp and release this library :)

MIDIController controller;
MIDIOut midiOut;

PeripheralHandle::Button myButton("button", PinAddress::GPIO_13);
PeripheralHandle::Potentiometer myPot("pot", PinAddress::GPIO_34);
PeripheralHandle::RotaryEncoder myEnc("enc", PinAddress::GPIO_18, PinAddress::GPIO_19, PinAddress::GPIO_21);

PeripheralHandle::Oled_SSD1306 myDisplay("oled");

// ComLink_ESPNow comLinkModule(ComRole::Module);
// ComLink_ESPNow comLinkHub(ComRole::Hub);
// bool bHub = true;

DisplayData *displayData = new DisplayData;

PeripheralHandle::Multiplexor mux(
    "mux",
    PinAddress::GPIO_5,    // S0
    PinAddress::GPIO_18,   // S1
    PinAddress::GPIO_19,   // S2
    PinAddress::GPIO_23,   // S3
    PinAddress::GPIO_32); 


int lastValue[16]; // temp for testing

PeripheralHandle::Button buttons[16] = {
    PeripheralHandle::Button("btn0"),  PeripheralHandle::Button("btn1"),  PeripheralHandle::Button("btn2"),  PeripheralHandle::Button("btn3"),
    PeripheralHandle::Button("btn4"),  PeripheralHandle::Button("btn5"),  PeripheralHandle::Button("btn6"),  PeripheralHandle::Button("btn7"),
    PeripheralHandle::Button("btn8"),  PeripheralHandle::Button("btn9"),  PeripheralHandle::Button("btn10"), PeripheralHandle::Button("btn11"),
    PeripheralHandle::Button("btn12"), PeripheralHandle::Button("btn13"), PeripheralHandle::Button("btn14"), PeripheralHandle::Button("btn15"),
};

void setup() {
  Serial.begin(115200);
  // Wire.begin(21, 22); // SDA, SCL
  
  for (uint8_t i = 0; i < 16; i++)
  {
      mux.attach(buttons[i], i);
      // controller.addControl(buttons[i]);
  }

  mux.setup();

  // controller.setOutput(midiOut);

  // controller.addControl(myButton);
  // controller.addControl(myPot);
  // controller.addControl(myEnc);
  // controller.addControl(myEnc, 20); // ControlHandle, cc number

  // controller.setup();

  // myDisplay.setDisplay(*displayData);
  // myDisplay.setup();

  // if (bHub)
  // {
  //   comLinkHub.begin();
  // }
  // else
  // {
  //   comLinkModule.begin();
  // }
  
}

void loop() {

  mux.update();

  // Todo: bitmask update to eliminate loop
  for (uint8_t i = 0; i < 16; i++)
    {
        int v = buttons[i].getValue();
        if (v == LOW && lastValue[i] != LOW)
            Serial.println(i);
        lastValue[i] = v;
    }

  // controller.update();
  // myDisplay.update();

  // for (int i = 1; i <= 100; i++) {

  //   if (i % 2 == 0)
  //     displayData->setBitmap(bmpL1A);
  //   else
  //     displayData->setBitmap(bmpSmile);

  //   myDisplay.setDisplay(*displayData);
  //   myDisplay.update();
  //   delay(1000);
  // }

  // int buttonState = myButton.getValue();
  // int potValuåe = myPot.getValue();
  // int encButtonValue = myEnc.getValueButton();
  // int encPosition = myEnc.getValuePosition();

  // Serial.printf("%d %d %d %d\n", encButtonValue, encPosition, potValue, buttonState);
  // controller.sendMidi();

  // if (bHub)
  // {
  //   comLinkHub.update();
  // }
  // else
  // {
  //   comLinkModule.update();
  // }

  delay(50);
}



