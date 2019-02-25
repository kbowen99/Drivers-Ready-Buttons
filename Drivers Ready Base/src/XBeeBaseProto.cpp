#pragma once
#include "storedSettings.h"
#include "magicMenu.h"
#include "XBeeBaseProto.h"
#include "main.h"
#include "buttonStates.h"

#define MESSAGE_PATTERN         0xB7
#define TIMEOUT                 3000
#define WARN_CHECKSUM           true

unsigned long lastMessageTime = 0;
unsigned long lastByteTime = 0;

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
 * Waits until serial available or timeout
 */
bool readWait() {
    while (Serial1.available() == 0 && (millis() - lastByteTime < TIMEOUT)) {
        delay(1);
    }
    if (Serial1.available()) {
        lastByteTime = millis();
        return true;
    }
    return false;
}

/**
 * Serial Reading code, should be called by serialEvent or SerialEvent1 (whicherver is applicable)
 */
void readSerial()
{
    lastByteTime = millis();
    while (Serial1.available())
    {
        byte inByte = Serial1.read();

        if (inByte == MESSAGE_PATTERN)
        {
            readWait();
            byte targetDevice = Serial1.read();
            if (targetDevice != getBaseStationAddr() || targetDevice != 0xFF) { return; }
            readWait();
            byte messageType = Serial1.read();

            switch (messageType)
            {
            case 0x02: //button press
                {
                    readWait();
                    byte buttonSent = Serial1.read();
                    readWait();
                    byte ChecksumByte = Serial1.read();
                    byte message[] = {0xB7, targetDevice, messageType, buttonSent};
                    if (ChecksumByte != checkSum(message, 4))
                    {
                        if (WARN_CHECKSUM)
                            updateMainStatus("badChecksum");
                        return;
                    }
                    //Test code
                    String tmp = "Btn ";
                    tmp = tmp + buttonSent;
                    tmp = tmp + " pressed";
                    updateMainStatus(tmp);
                    receivedAddr(buttonSent);
                }
                break;
            case 0x06: //gameOver
                break;
            case 0x08: //battery status, ignored by other buttons
                {
                    readWait();
                    byte buttonSent = Serial1.read();
                    readWait();
                    byte battStatus = Serial1.read();
                    readWait();
                    byte ChecksumByte = Serial1.read();
                    byte message[] = {0xB7, targetDevice, messageType, buttonSent, battStatus};
                    if (ChecksumByte != checkSum(message, 5))
                    {
                        updateMainStatus("badVBSum");
                        return;
                    }
                    buttonVoltages[buttonToIndex(buttonSent)] = ((battStatus / 255.0F) * 5.0F);
                    buttonLastBroadcast[buttonToIndex(buttonSent)] = millis();
                    receivedAddr(buttonSent);
                }
                break;
            case 0x0A: //ACK
                break;
            case 0x00: //not implemented
            case 0x01: //setImage Message
            case 0x03: //Set Team Color
            case 0x04: //match start
            case 0x05: //Final Countdown
            case 0x07: //powerdown
            case 0x09: //rainBow
            case 0x0B: //Arm
                break;
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