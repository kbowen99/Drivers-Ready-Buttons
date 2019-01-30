#pragma once
#include <Arduino.h>

void resetAll();
bool storedSettings();
bool getJudgeButton();
void setJudgeButton(bool e);
uint8_t getJudgeSA();
void setJudgeSA(uint8_t sa);
uint8_t getDriver1SA();
void setDriver1SA(uint8_t sa);
uint8_t getDriver2SA();
void setDriver2SA(uint8_t sa);
uint8_t getDriver3SA();
void setDriver3SA(uint8_t sa);
uint8_t getDriver4SA();
void setDriver4SA(uint8_t sa);
uint8_t getNumberDrivers();
void setNumberButtons(uint8_t n);