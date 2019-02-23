#include <Arduino.h>
#include "XBeeProto.h"
#include "neoDraw.h"
#include "main.h"
#include "storedSettings.h"

#define BUTTON_PIN              3
#define BUTTON_TIMEOUT          500
#define BUTTON_HOLD_DURATION    15000.0F
#define DEBUG_OUTPUT            FALSE
#define BATTERY_VOLTAGE_PIN     A0
#define MAX_BATTERY_VOLTAGE     4.2
#define MIN_BATTERY_VOLTAGE     3.0
#define LOW_BATTERY_VOLTAGE     3.2
#define MAX_ARDUINO_ADC         1023
#define MAX_ARDUINO_VOLTAGE     5.0

uint32_t currentImage[NUMPIXELS];
bool imageChanged = false;

unsigned long lastButtonSmash = 0;
bool lastButtonSmashState = false;
bool lastHold = false;
short scaledBatteryVoltage = 0;

void buttonSmash() {
  if ((millis() - lastButtonSmash) > BUTTON_TIMEOUT && !lastHold){
    lastButtonSmash = millis();
    setSpiralColor(getColorR(),getColorG(),getColorB());
    send_Button_Pressed();
    setSpiralColor(0,0,0);
  }

  if (lastHold) {
    setColor(0,0,0);
    lastHold = false;
  }
  
  lastButtonSmashState = digitalRead(BUTTON_PIN);
}

void setup() {
  //setColorR(0);setColorG(255);setColorB(0);setUseStored(true);setButtonID(0x2);
  setupNeopixel();
  setSpiralColor(getColorR(),getColorG(),getColorB());
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),buttonSmash,CHANGE);

  //rainbowCycle(2);
  setSpiralColor(0,0,0);
}

void loop() {
  //draw();
  updateVbatt();

  if (!lastButtonSmashState && !digitalRead(BUTTON_PIN) && ((millis() - lastButtonSmash) > BUTTON_TIMEOUT)) {
    if ((millis() - lastButtonSmash) < BUTTON_HOLD_DURATION) {
      float delta = millis() - lastButtonSmash;
      setSpiralPercentage(delta / BUTTON_HOLD_DURATION);
      lastHold = true;
    } else {
      setColor(255,0,0);
    }
    delay(10);
  }
}

void serialEvent() {
  readSerial();
}

/**
 * Maps the current battery voltage from MIN_ARDUINO_VOLTAGE to MAX_ARDUINO_VOLTAGE --> 0 to 255
 */
void updateVbatt() {
  //scaledBatteryVoltage = map(analogRead(BATTERY_VOLTAGE_PIN), (MIN_BATTERY_VOLTAGE * (MAX_ARDUINO_ADC / MAX_ARDUINO_VOLTAGE)), MAX_ARDUINO_ADC, 0x00, 0xFF);
  scaledBatteryVoltage = map(analogRead(BATTERY_VOLTAGE_PIN), 0, MAX_ARDUINO_ADC, 0x00, 0xFF);
}