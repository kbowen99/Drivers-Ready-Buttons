#pragma once
#include <Arduino.h>
#include "magicMenu.h"
#include "storedSettings.h"

#define MATCH_TIME_MILLIS       3*60*1000
#define ARMING_TIME             5*1000
#define DEBUG_SERIAL            true

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

/**
 * Eric's Checksum implementation, should help a bit with data loss
 */
byte checkSum(byte message[], int numBytes) {
  byte total = 0x00;
  for (int i = 0; i < numBytes; i++) {
    total += message[i];
  }
  return total;
}

void serialEvent1(){
   while (Serial1.available())
    {
        byte inByte = Serial1.read();
        
        if (inByte == 0xB7)
        {
            while (Serial1.available() == 0){delay(1);}
            byte targetDevice = Serial1.read();
            while (Serial1.available() == 0){delay(1);}
            byte messageType = Serial1.read();
            while (Serial1.available() == 0){delay(1);}
            byte buttonSent = Serial1.read();
            while (Serial1.available() == 0) { delay(1); }
            byte ChecksumByte = Serial1.read();
            byte message[] = {0xB7, targetDevice, messageType, buttonSent};
            if (ChecksumByte != checkSum(message, 4)) { updateMainStatus("badChecksum");return; }
            String disp = "Btn ";
            disp = disp + (int)buttonSent;
            disp = disp + " Pressed";
            updateMainStatus(disp);
            receivedAddr(buttonSent);
        }
    }
}


void loop() {
  updateLCD();
}
