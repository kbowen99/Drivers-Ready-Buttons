#define neopixelPin     4
#define buttonPin       3
#define NUMPIXELS      12
#define ARDUINOISSHIT   5

#include <Adafruit_Neopixel.h>
#include <avr/power.h>
#include <Arduino.h>
#define MESSAGE_LENGTH 39
#define buttonID 0x01

#define MESSAGE_PATTERN 0xB7

uint32_t currentImage[NUMPIXELS];
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neopixelPin, NEO_GRB + NEO_KHZ800);
bool imageChanged = false;

long lastMessageTime;
int messageTimeout = 300;

void send_Button_Pressed() {
  // byte message[4];
  // message[0] = MESSAGE_PATTERN;
  // message[1] = 0x00;
  // message[2] = 0x02;
  // message[3] = buttonID;

  //Serial.write(message,4);
  Serial.write(MESSAGE_PATTERN);
  Serial.write(0xFF);
  Serial.write(0x02);
  Serial.write(buttonID);
}

int spiralize_delay = 50;
int buttonInst = 2;

void setSpiralColor(int r, int g, int b){
  for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(r,g,b));
      pixels.show();
      delay(spiralize_delay);
    } 
}

//extracts data after message pattern is matched
void Parse_DRB_Message(byte message[]) {
  //parse message type (byte 1)
  byte messageType = message[1];

  switch (messageType){
    case (0x01):
      //parse image data (bytes 2-37)
      for (short i = 0; i < NUMPIXELS; i++) {
        int index = (3*i)+2;
        currentImage[i] = (((uint32_t)int(message[index]) << 16) | ((uint32_t)int(message[index+1]) <<  8) | (uint8_t)int(message[index+2]));
      }
      
      imageChanged = true;
      break;

    default:
      break;
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

// /**
//  * XBee Reading Code
//  */
// void SerialEvent() {
//   hardcodedColors(Serial.read());
// }

void serialEvent() {
  while (Serial.available()) {
    byte inByte = Serial.read();

    if (inByte == MESSAGE_PATTERN) {
      byte message[MESSAGE_LENGTH];

      Serial.readBytesUntil(';', message, MESSAGE_LENGTH);  //; is 3B
      
      if (message[0] != buttonID) { return; }

      Parse_DRB_Message(message);
    }
  }
}


/**
 * Code to handle smashing of driver button
 */
void buttonSmash() {
  // if (!digitalRead(buttonPin)) {
  //   Serial.write(buttonInst);
  //   hardcodedColors(buttonInst);
  // }
  send_Button_Pressed();
}

void draw(){
  if (imageChanged) {
    imageChanged = false;
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, currentImage[i]);
    }
    pixels.show();
  }
}

void loop() {
  draw();
}

void setup() {
  //XBee Communication
  Serial.begin(115200);
  Serial.setTimeout(250);
  //Initialize Neopixel Ring
  pixels.begin();
  //Setup Button Input
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin),buttonSmash,CHANGE);
  //delay(750);
  setSpiralColor(100,100,100);
}

