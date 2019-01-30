#pragma once
#include <Arduino.h>
#include "main.h"
#include "magicMenu.h"
#include "storedSettings.h"

#define MATCH_TIME_MILLIS       3*60*1000
#define ARMING_TIME             5*1000

void setup() {
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
            String disp = "Button ";
            disp = disp + (int)buttonSent;
            disp = disp + " Pressed";
            updateMainStatus(disp);
        }
    }
}

void loop() {
  updateLCD();
}
