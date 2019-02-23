#pragma once
#include "storedSettings.h"
#include "magicMenu.h"
#include "XBeeBaseProto.h"

#define MESSAGE_PATTERN 0xB7
#define TIMEOUT 10000

unsigned long lastMessageTime = 0;

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
 * display string for button press
 */
String btnDisp(int number) {
    String tmp = "Button ";
    tmp = tmp + number;
    tmp = tmp + " pressed";
    return tmp;
}

/**
 * Serial Reading code, should be called by serialEvent or SerialEvent1 (whicherver is applicable)
 */
void readSerial()
{
    while (Serial1.available())
    {
        byte inByte = Serial1.read();

        if (inByte == MESSAGE_PATTERN)
        {
            while (Serial1.available() == 0)
            {
                delay(1);
            }
            byte targetDevice = Serial1.read();
            if (targetDevice != getBaseStationAddr() || targetDevice != 0xFF)
            {
                return;
            }

            while (Serial1.available() == 0)
            {
                delay(1);
            }
            byte messageType = Serial1.read();

            switch (messageType)
            {
            case 0x02: //button press
                while (Serial1.available() == 0)
                {
                    delay(1);
                }
                byte buttonSent = Serial1.read();
                while (Serial1.available() == 0)
                {
                    delay(1);
                }
                byte ChecksumByte = Serial1.read();
                byte message[] = {0xB7, targetDevice, messageType, buttonSent};
                if (ChecksumByte != checkSum(message, 4))
                {
                    updateMainStatus("badChecksum");
                    return;
                }
                //Test code
                updateMainStatus(btnDisp(buttonSent));
                receivedAddr(buttonSent);
                break;
            case 0x06: //gameOver
                break;
            case 0x08: //battery status, ignored by other buttons
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