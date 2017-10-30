/*******************************************************************
 *  A project to trigger your phone's camera using bluetooth       
 *  
 *  Main Hardware:
 *  - ESP8266 (any Arduino will work, but might require hardware serial
 *  - HC-06 flashed with RN-42 Firmware (https://youtu.be/y8PcNbAA6AQ)
 *  - Any type of button
 *                                                                 
 *  Written by Brian Lough                                         
 *******************************************************************/

#include <DoubleResetDetector.h>

#include <BPLib.h>
#include <SoftwareSerial.h>

#define RX_PIN D6 // connect to TXD of module
#define TX_PIN D5 // connect to RXD of module (logic level 3.3v!)

#define BUTTON D7

DoubleResetDetector drd(10, 0);

int buttonCoolDownDue = 0;

BPLib *BPMod;

SoftwareSerial swSer(RX_PIN, TX_PIN, false, 128);

void setup() {
  Serial.begin(115200);
  bool doubleResetDetected = drd.detectDoubleReset();
  swSer.begin(115200);

  pinMode(BUTTON, INPUT_PULLUP);

  BPMod = new BPLib(Serial);

  if (doubleResetDetected) {
    BPMod->begin(BP_MODE_HID,BP_HID_KEYBOARD);
  }
}

void loop() {
  drd.loop();
  int time = millis();
  if (time > buttonCoolDownDue) {
    if (digitalRead(BUTTON) == LOW) {
      BPMod->volumeUp();
      delay(100);
      BPMod->keyRelease();
      buttonCoolDownDue = time + 500;
    }
  }

}
