#pragma once
#include <Arduino.h>

void readSerial();
byte checkSum(byte message[], int numBytes);
void send_serial_spaced(byte m[], int l);