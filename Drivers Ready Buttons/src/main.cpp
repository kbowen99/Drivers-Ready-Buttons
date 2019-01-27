#include <Arduino.h>
#include <Adafruit_Neopixel.h>
#include <avr/power.h>

#define neopixelPin     4
#define buttonPin       3
#define NUMPIXELS      12

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neopixelPin, NEO_GRB + NEO_KHZ800);

int spiralize_delay = 50;
int buttonInst = 2;


void setup() {
  //XBee Communication
  Serial.begin(115200);
  //Initialize Neopixel Ring
  pixels.begin();
  //Setup Button Input
  pinMode(buttonPin, INPUT_PULLUP);
}

void setSpiralColor(int r, int g, int b){
  for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(r,g,b));
      pixels.show();
      delay(spiralize_delay);
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
    default:
    setSpiralColor(0,0,0);
  }
  
}

void loop() {
  //Shitty Test Code
  if (Serial.available()) {
    hardcodedColors(Serial.read());
  } else if (!digitalRead(buttonPin)) {
    Serial.write(buttonInst);
    hardcodedColors(buttonInst);
  }
}
