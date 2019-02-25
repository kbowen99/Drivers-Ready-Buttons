#pragma once
#include <Arduino.h>

#define HIGH_BATTERY_VOLTAGE        4.3
#define MIN_BATTERY_VOLTAGE         3.0
#define LOW_BATTERY_VOLTAGE         3.4
#define BUTTON_LOST_TIMEOUT         15*1000

extern uint8_t addressBuffer[];
extern float buttonVoltages[6];
extern unsigned long buttonLastBroadcast[6];

String indexDescriptor(int index);
uint8_t indexToButton(int index);
int buttonToIndex(uint8_t SA);
void receivedAddr(int addr);
void updateButtonStates();