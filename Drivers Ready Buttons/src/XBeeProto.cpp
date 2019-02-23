#pragma once
#include "main.h"
#include "XBeeProto.h"
#include <Arduino.h>
#include "neoDraw.h"
#include <avr/power.h>
#include "storedSettings.h"

/**
 * Messages generally follow the format of:
 * MESSAGE_PATTERN, target device, message type, data(s), checksum()
 */

#define MESSAGE_LENGTH 40
#define MESSAGE_PATTERN 0xB7
#define TIMEOUT 10000

long lastMessageTime;

/**
 * Registers Button Press over the Network
 */
void send_Button_Pressed()
{
    byte message[] = {MESSAGE_PATTERN, 0xFF, 0x02, getButtonID()};
    send_serial_spaced(message, 4);
    Serial.write(checkSum(message, 4));
}

/**
 * Gameover message sent as button tap out
 */
void send_button_gameOver()
{
    byte message[] = {MESSAGE_PATTERN, 0xFF, 0x06, getButtonID()};
    send_serial_spaced(message, 4);
    Serial.write(checkSum(message, 4));
}

/**
 * Current battery status, and button 'pulse'
 */
void send_battery_status(byte vBatt)
{
    byte message[] = {MESSAGE_PATTERN, 0xFF, 0x08, getButtonID(), vBatt};
    send_serial_spaced(message, 5);
    Serial.write(checkSum(message, 5));
}

/**
 * Sends the message array with safe time delays
 */
void send_serial_spaced(byte m[], int l)
{
    for (int i = 0; i < l; i++)
    {
        Serial.write(m[i]);
        delay(10);
    }
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

            switch (messageType)
            {
            case 0x01: // setImage Message
                byte message[NUMPIXELS * 3];

                unsigned long startTime = millis();
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
                while (Serial.available() == 0)
                {
                    delay(1);
                }
                byte ChecksumByte = Serial.read();
                if (ChecksumByte != checkSum(message, NUMPIXELS * 3))
                {
                    return;
                }
                Parse_DRB_Message(message);
                break;
            case 0x03://Set Team Color
                break;
            case 0x04://match start
                break;
            case 0x05://Final Countdown
                break;
            case 0x06://gameOver
                break;
            case 0x07://powerdown
                break;
            case 0x09://rainBow
                break;
            case 0x0A://ACK
                break;
            case 0x0B://Arm
                break;
            case 0x00://not implemented
            case 0x02://button press, ignored by other buttons
            case 0x08://battery status, ignored by other buttons
            default:
                break;
            }
        }
    }
}

/**
 * Eric's Checksum implementation, should help a bit with data loss
 */
byte checkSum(byte message[], int numBytes)
{
    byte total = 0x00;
    for (int i = 0; i < numBytes; i++)
    {
        total += message[i];
    }
    return total;
}