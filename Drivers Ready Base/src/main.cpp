#pragma once
#include <Arduino.h>
#include "magicMenu.h"
#include "storedSettings.h"
#include "XBeeBaseProto.h"
#include "main.h"

#define MATCH_TIME_MILLIS           3*60*1000
#define ARMING_TIME                 5*1000
#define DEBUG_SERIAL                true
#define DEBUG_BROADCAST_TIMEOUT     10
#define BUTTON_LOST_TIMEOUT         15*1000
#define HIGH_BATTERY_VOLTAGE        4.3
#define MIN_BATTERY_VOLTAGE         3.0
#define LOW_BATTERY_VOLTAGE         3.4

long lastBroadcastTime;

/**
 * Converted battery voltage received from each button
 */
float buttonVoltages[] = {5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 0.0F};

/**
 * Last time a message was received from a button
 */
unsigned long buttonLastBroadcast[] = {0, 0, 0, 0, 0, 0};

/**
 * Whether or not each button is believed to be online
 */
bool buttonOnline[] = {false, false, false, false, false, false};

/**
 * returns the number of online buttons
 */
int numberOnlineButtons() {
  int tmp = 0;
  for (int i = 0; i < getNumberDrivers(); i++) {
    tmp += buttonOnline[i];
  }
  return tmp;
}

/**
 * Maps each index to its SA
 */
uint8_t indexToButton(int index) {
  switch (index) {
    case 0:
      return getJudgeSA();
    case 1:
      return getDriver1SA();
    case 2:
      return getDriver2SA();
    case 3:
      return getDriver3SA();
    case 4:
      return getDriver4SA();
    default:
      break;
  }
}

/**
 * Returns a human readable descriptor for each button index
 */
String indexDescriptor(int index) {
  switch (index)
  {
    case 0:
      return "Judge";
    case 1:
    case 2:
    case 3:
    case 4:
      return "Driver " + index;
    default:
      return "Unknown";
  }
}


/**
 * Maps a buttons SA to its index (Judge = 0, Driver 1 = 1, Driver 2 = 2, etc.)
 */
int buttonToIndex(uint8_t SA) {
  for (int i = 0; i < getNumberDrivers(); i++)
    if (SA == indexToButton(i))
      return i;
  return getNumberDrivers();
}

void setup() {
  setupLCD();
  updateMainStatus("Starting Serial.");
  Serial.begin(115200);
  Serial1.begin(115200);
  while(!Serial1){delay(1);}

  if (storedSettings() && getNumberDrivers() > 1)
    updateMainStatus("Ready!");
  else
    updateMainStatus("Program Buttons!");

  if (DEBUG_SERIAL){
    printParametersToSerial();
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

  if (storedSettings() && getNumberDrivers() > 1) {
    for (int i = 0; i < getNumberDrivers(); i++) {
      if (buttonVoltages[i] > 0 &&  buttonVoltages[i] < LOW_BATTERY_VOLTAGE) {
        String tmp = indexDescriptor(i) + " ";
        tmp = tmp + buttonVoltages[i];
        tmp = tmp + "v";
        updateMainStatus(tmp);
      }
    }
  }
}

/**
 * Prints EEPROM parameters to Serial for debugging
 */
void printParametersToSerial() {
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