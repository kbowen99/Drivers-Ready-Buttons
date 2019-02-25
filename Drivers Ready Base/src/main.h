#pragma once
#include <Arduino.h>

extern float buttonVoltages[6];
extern unsigned long buttonLastBroadcast[6];

String indexDescriptor(int index);
uint8_t indexToButton(int index);
int buttonToIndex(uint8_t SA);
void printParametersToSerial();