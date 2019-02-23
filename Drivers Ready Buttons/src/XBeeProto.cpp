#pragma once
#include "main.h"
#include "XBeeProto.h"
#include <Arduino.h>
#include "neoDraw.h"
#include <avr/power.h>
#include "storedSettings.h"

#define MESSAGE_LENGTH      40
#define MESSAGE_PATTERN     0xB7
#define TIMEOUT             10000

long lastMessageTime;

/**
 * Registers Button Press over the Network
 */
void send_Button_Pressed()
{
    Serial.write(MESSAGE_PATTERN);
    delay(10);
    Serial.write(0xFF);
    delay(10);
    Serial.write(0x02);
    delay(10);
    Serial.write(getButtonID());
    delay(10);
    byte message[] = {MESSAGE_PATTERN, 0xFF, 0x02, getButtonID()};
    Serial.write(checkSum(message,4));
}

/**
 * Parses Drivers Ready Button Commanded image message
 */
void Parse_DRB_Message(byte message[])
{
    for (short i = 0; i < NUMPIXELS; i++)
    {
        int index = (3 * i);
        currentImage[i] = (((uint32_t) int(message[index]) << 16) | ((uint32_t) int(message[index + 1]) << 8) | (uint32_t) int(message[index + 2]));
    }
    imageChanged = true;
    draw();
}

/**
 * Serial Reading code, should be called by serialEvent or SerialEvent1 (whicherver is applicable)
 */
void readSerial()
{
    while (Serial.available())
    {
        byte inByte = Serial.read();
        
        if (inByte == MESSAGE_PATTERN)
        {
            while (Serial.available() == 0)
            {
                delay(1);
            }
            byte targetDevice = Serial.read();
            if (targetDevice != getButtonID())
            {
                return;
            }

            while (Serial.available() == 0)
            {
                delay(1);
            }
            byte messageType = Serial.read();

            byte message[NUMPIXELS * 3];

            long startTime = millis();
            int index = 0;
            while (millis() < startTime + TIMEOUT)
            {
                
                delay(1);
                while ((Serial.available() == 0) && (millis() < startTime + TIMEOUT))
                {
                    delay(1);
                }
                message[index] = Serial.read();
                index++;
                if (index == NUMPIXELS * 3)
                {
                    break;
                }
            }
            if (index != (NUMPIXELS * 3))
            {
                Serial.print("ERR ");
                Serial.println(index);
                Serial.flush();
                return;
            }
            while (Serial.available() == 0) { delay(1); }
            byte ChecksumByte = Serial.read();
            if (ChecksumByte != checkSum(message, NUMPIXELS*3)) { return; }
            Parse_DRB_Message(message);
        }
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