#include <Arduino.h>
#include "XBeeProto.h"
#include "neoDraw.h"
#include "main.h"
#include "storedSettings.h"

#define BUTTON_PIN       3
#define BUTTON_TIMEOUT    750

uint32_t currentImage[NUMPIXELS];
bool imageChanged = false;

unsigned long lastButtonSmash = 0;

void buttonSmash() {
  if ((millis() - lastButtonSmash) > BUTTON_TIMEOUT){
    lastButtonSmash = millis();
    setSpiralColor(getColorR(),getColorG(),getColorB());
    send_Button_Pressed();
    setSpiralColor(0,0,0);
  }
}

void setup() {
  //setColorR(0);setColorG(0);setColorB(0);setUseStored(true);
  setupNeopixel();
  setSpiralColor(getColorR(),getColorG(),getColorB());
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),buttonSmash,CHANGE);
  setSpiralColor(0,0,0);
}

void loop() {
  draw();
}

void SerialEvent() {
  readSerial();
}
