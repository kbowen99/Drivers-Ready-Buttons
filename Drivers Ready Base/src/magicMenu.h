#pragma once
#include <Arduino.h>

void setupLCD();
void readButtons();
void updateLCD();
void navigateMenus();
void drawCurrent();
void updateMainStatus(String s);
void receivedAddr(uint8_t addr);