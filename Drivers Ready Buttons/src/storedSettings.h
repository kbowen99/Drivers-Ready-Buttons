#pragma once
#include <Arduino.h>

void resetAll();
bool useStoredSettings();
void setUseStored(bool b);
void setColorR(int_fast8_t r);
void setColorG(int_fast8_t c);
void setColorB(int_fast8_t c);
uint8_t getColorR();
uint8_t getColorG();
uint8_t getColorB();