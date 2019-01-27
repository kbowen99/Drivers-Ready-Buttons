#include <Arduino.h>
#include <Adafruit_Neopixel.h>
#include <avr/power.h>

#define neopixelPin     4
#define buttonPin       3
#define NUMPIXELS      12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neopixelPin, NEO_GRB + NEO_KHZ800);

int spiralize_delay = 50;
int buttonInst = 9;

void setSpiralColor(int r, int g, int b){
  for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(r,g,b));
      pixels.show();
      delay(spiralize_delay);
    } 
}

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

void hardcodedColors(int s){
  switch (s){
    case 0:
    setSpiralColor(255,255,255);
    break;
    case 1:
    setSpiralColor(255,0,0);
    break;
    case 2:
    setSpiralColor(0,255,0);
    break;
    case 3:
    setSpiralColor(0,0,255);
    break;
    case 9:
    rainbowCycle(20);
    break;
    default:
    setSpiralColor(0,0,0);
  }
  
}

/**
 * XBee Reading Code
 */
void SerialEvent() {
  hardcodedColors(Serial.read());
}

/**
 * Code to handle smashing of driver button
 */
void buttonSmash() {
  if (!digitalRead(buttonPin)) {
    Serial.write(buttonInst);
    hardcodedColors(buttonInst);
  }
}

void loop() {}

void setup() {
  //XBee Communication
  Serial.begin(115200);
  //Initialize Neopixel Ring
  pixels.begin();
  //Setup Button Input
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin),buttonSmash,CHANGE);
}

