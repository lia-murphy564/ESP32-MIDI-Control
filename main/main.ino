#include <Arduino.h>
#include "pinout_esp32.h"
#include "pin_handle.h"
#include "Peripheral.h"
#include "Bluetooth.h"
#include "MIDIControl.h"
#include "Communication.h"
#include "Multiplexor.h"
#include "bitmap.h"
#include "LMUtil.h"
#include "LMThread.h"

// NOTE: implementation is inside .h files for easy prototype development. I know i knowwww but this compiles and works. Todo: Move implementation to .cpp and release this library :)

MIDIController controller;
MIDIOut midiOut;

// Todo: abstract into controldata class
Peripheral::Button myButton("button", PinAddress::GPIO_13);
Peripheral::Potentiometer myPot("pot", PinAddress::GPIO_34);
Peripheral::RotaryEncoder myEnc("enc", PinAddress::GPIO_18, PinAddress::GPIO_19, PinAddress::GPIO_21);

Peripheral::Oled_SSD1306 myDisplay("oled");

Peripheral::LED myledblue("ledblue", PinAddress::GPIO_25);
Peripheral::LED myledwhite("ledwhite", PinAddress::GPIO_26);
Peripheral::LED myledgreen("ledgreen", PinAddress::GPIO_27, true);


// Testing
// ComLink_ESPNow comLinkModule(ComRole::Module);
// ComLink_ESPNow comLinkHub(ComRole::Hub);
// bool bHub = true;

DisplayData *displayData = new DisplayData;

Peripheral::Multiplexor mux(
    "mux",
    PinAddress::GPIO_5,    // S0
    PinAddress::GPIO_18,   // S1
    PinAddress::GPIO_19,   // S2
    PinAddress::GPIO_23,   // S3
    PinAddress::GPIO_32); 


int lastValue[16]; // temp for testing

Peripheral::Button buttons[16] = {
    Peripheral::Button("btn0"),  Peripheral::Button("btn1"),  Peripheral::Button("btn2"),  Peripheral::Button("btn3"),
    Peripheral::Button("btn4"),  Peripheral::Button("btn5"),  Peripheral::Button("btn6"),  Peripheral::Button("btn7"),
    Peripheral::Button("btn8"),  Peripheral::Button("btn9"),  Peripheral::Button("btn10"), Peripheral::Button("btn11"),
    Peripheral::Button("btn12"), Peripheral::Button("btn13"), Peripheral::Button("btn14"), Peripheral::Button("btn15"),
};

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA, SCL
  
  for (uint8_t i = 0; i < 16; i++)
  {
      mux.attach(buttons[i], i);
      // controller.addControl(buttons[i]);
  }

  mux.setup();
  myledblue.setup();
  myledwhite.setup();
  myledgreen.setup();

  myledblue.setBlink(500);
  myledwhite.setBlink(1000);

  myledgreen.setBlink(10);
  myledgreen.set(1);


  // controller.setOutput(midiOut);

  // controller.addControl(myButton);
  // controller.addControl(myPot);
  // controller.addControl(myEnc);
  // controller.addControl(myEnc, 20); // ControlHandle, cc number

  // controller.setup();

  // myDisplay.setDisplay(*displayData);
  // myDisplay.setup();

  // Testing
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

//   mux.update();

  myledblue.update();
  myledwhite.update();
  myledgreen.update();


//   uint16_t period = 10;
//   static uint32_t lastStep = 0;
//   static uint8_t brightness = 0;
//   static int8_t direction = 1;

//   if (millis() - lastStep >= period)
//   {
//     lastStep = millis();

//     brightness += direction;
//     if (brightness == 0 || brightness >= 255)
//       direction = -direction;

//     myledgreen.pwm(brightness);
//   }


//   // Todo: bitmask update to eliminate loop
//   for (uint8_t i = 0; i < 16; i++)
//     {
//         int v = buttons[i].getValue();
//         if (v == LOW && lastValue[i] != LOW)
//             Serial.println(i);
//         lastValue[i] = v;
//     }

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
  // int potValue = myPot.getValue();
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

//   delay(50);
}



