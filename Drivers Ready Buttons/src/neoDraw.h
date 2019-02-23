#pragma once
#include <Arduino.h>

#define NUMPIXELS               12
#define NEOPIXEL_PIN            4
#define SPIRALIZE_DELAY         50

void setupNeopixel();
void draw();
uint32_t Wheel(byte WheelPos);
void rainbowCycle(uint8_t wait);
void setSpiralColor(int r, int g, int b);
void setSpiralPercentage(float val);
void setColor(int r, int g, int b);