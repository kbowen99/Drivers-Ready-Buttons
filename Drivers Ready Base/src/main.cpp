#pragma once
#include <Arduino.h>
#include "magicMenu.h"
#include "storedSettings.h"
#include "XBeeBaseProto.h"

#define MATCH_TIME_MILLIS         3*60*1000
#define ARMING_TIME               5*1000
#define DEBUG_SERIAL              true
#define DEBUG_BROADCAST_TIMEOUT   10

long lastBroadcastTime;

void setup() {
  //resetAll();

  setupLCD();
  updateMainStatus("Starting Serial.");
  Serial.begin(115200);
  Serial1.begin(115200);
  while(!Serial1){delay(1);}
  delay(500);
  if (storedSettings() && getNumberDrivers() > 1)
    updateMainStatus("Ready.");
  else
    updateMainStatus("Program Buttons!");

  if (DEBUG_SERIAL){
    Serial.println("ROBOCONTROL Parameters");
    Serial.print("Using EEPROM: ");
    Serial.println(storedSettings());
    Serial.print("Using Judge Button: ");
    Serial.println(getJudgeButton());
    Serial.print("Number of Drivers: ");
    Serial.println(getNumberDrivers());
    Serial.print("JBtn: ");
    Serial.println(getJudgeSA());
    Serial.print("DRBtn1: ");
    Serial.println(getDriver1SA());
    Serial.print("DRBtn2: ");
    Serial.println(getDriver2SA());
    Serial.print("DRBtn3: ");
    Serial.println(getDriver3SA());
    Serial.print("DRBtn4: ");
    Serial.println(getDriver4SA());
  }
}

void serialEvent(){
  if (DEBUG_SERIAL){
    if (!(millis() > lastBroadcastTime + DEBUG_BROADCAST_TIMEOUT)){
      delay(DEBUG_BROADCAST_TIMEOUT);
    }
    Serial1.write((byte)Serial.read());
    lastBroadcastTime = millis();
  }
}

/**
 * Serial Event interrupt
 */
void serialEvent1(){
  readSerial();
}


void loop() {
  updateLCD();
}
