#include <Arduino.h>
#include "pinout_esp32.h"
#include "pin_handle.h"
#include "Control.h"
#include "Bluetooth.h"
#include "MIDIControl.h"

MIDIController controller;

Button myButton(PinAddress::GPIO_13);
Potentiometer myPot(PinAddress::GPIO_34);
RotaryEncoder myEnc(PinAddress::GPIO_18, PinAddress::GPIO_19, PinAddress::GPIO_21);

void setup() {
  Serial.begin(9600);

  // myPin.setup();
  myButton.setup();
  myPot.setup();
  myEnc.setup();


  controller.setup();
  controller.addControl(myButton);
}

void loop() {
  // int buttonState = myPin.read();
  myButton.update();
  myPot.update();
  myEnc.update();

  int buttonState = myButton.getValue();
  int potValue = myPot.getValue();
  int encButtonValue = myEnc.getValueButton();

  controller.update();

  Serial.println(encButtonValue);
  // delay(50);

  // When pressed, pin goes LOW
  // if (buttonState == LOW) {
  //   Serial.println("The button is pressed");
  //   delay(200);
  // }
  // else
  // {
  //   Serial.println("----");
  //   delay(200);
  // }
}



