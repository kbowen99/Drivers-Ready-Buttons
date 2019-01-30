#pragma once
#include "main.h"
#include "neoDraw.h"
#include "XBeeProto.h"
#include <Adafruit_NeoPixel.h>
#include <utility/Adafruit_MCP23017.h>
#include <Wire.h>

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

/**
 * Initializes the Neopixel Object, and starts up the lights
 */
void setupNeopixel() {
    pixels.begin();
}

/**
 * Draws the current image (if the image changed) on the neopixel
 */
void draw(){
  if (imageChanged) {
    imageChanged = false;
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, currentImage[i]);
    }
    pixels.show();
  }
}

/**
 * Generates a rainbow wheel to send to neopixel
 */
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

/**
 * Spins a rainbow wheel.
 * THIS IS A BLOCKING FUNCTION
 */
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

/**
 * Spins a color onto the neopixel
 * THIS IS A BLOCKING FUNCTION
 */
void setSpiralColor(int r, int g, int b){
  for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(r,g,b));
      pixels.show();
      delay(SPIRALIZE_DELAY);
    } 
}