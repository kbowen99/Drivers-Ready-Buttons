#pragma once
#include "main.h"
#include "XBeeProto.h"
#include <Arduino.h>
#include "neoDraw.h"
#include <avr/power.h>

#define buttonID            0x03
#define MESSAGE_LENGTH      40
#define MESSAGE_PATTERN     0xB7
#define TIMEOUT             1000

long lastMessageTime;

/**
 * Registers Button Press over the Network
 */
void send_Button_Pressed()
{
    Serial.write(MESSAGE_PATTERN);
    Serial.write(0xFF);
    Serial.write(0x02);
    Serial.write(buttonID);
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
            if (targetDevice != buttonID)
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
                while (Serial.available() == 0)
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

            if (index != 36)
            {
                Serial.print("ERR");
                Serial.flush();
                return;
            }

            Parse_DRB_Message(message);
        }
    }
}
