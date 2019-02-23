#pragma once
#include "neoDraw.h"

extern bool imageChanged;
extern uint32_t currentImage[NUMPIXELS];

/**
 * Button operating states
 */
enum buttonState {IDLE, ARMED, GAME, GAMEOVER};

void setup();
void loop();
void SerialEvent();
void updateVbatt();