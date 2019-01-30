#pragma once
#include "neoDraw.h"

extern bool imageChanged;
extern uint32_t currentImage[NUMPIXELS];

void setup();
void loop();
void SerialEvent();