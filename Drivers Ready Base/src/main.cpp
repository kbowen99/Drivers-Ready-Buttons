#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

void setup() {
  lcd.begin(16, 2);
  lcd.print("Robocontrol V0");
  lcd.setBacklight(0x7);

  Serial.begin(115200);

  Serial1.begin(115200);
  while(!Serial1){}
  lcd.clear();
  lcd.print("Ready");
  pinMode(13, OUTPUT);
}

#define MESSAGE_PATTERN 0xB7
#define MESSAGE_LENGTH 40

#define buttonID 0x01
#define NUMPIXELS 12
uint32_t currentImage[NUMPIXELS];
bool imageChanged = false;

void Parse_DRB_Message(byte message[]) {
  for (short i = 0; i < NUMPIXELS; i++) {
    int index = (3*i);
    Serial.print("R: ");
    Serial.print(int(message[index]));
    Serial.print("\tG: ");
    Serial.print(int(message[index+1]));
    Serial.print("\tB: ");
    Serial.println(int(message[index+2]));

    currentImage[i] = (((uint32_t)int(message[index]) << 16) | ((uint32_t)int(message[index+1]) <<  8) | (uint8_t)int(message[index+2]));
  }
      
  imageChanged = true;

}

#define TIME 100

void loop() {
  //uint8_t buttons = lcd.readButtons();

  for (int i = 0; i < 12; i++) {
    Serial1.write(0xB7);
    Serial1.write(0x01);
    Serial1.write(0x01);
    for (int j = 0; j < i; j++) {
      Serial1.write(0x000000);
    }
    Serial1.write(0xFFFFFF);
    for (int j = i+1; j < 12; j++) {
      Serial1.write(0x000000);
    }
    digitalWrite(13, HIGH);
    delay(TIME);
    digitalWrite(13, LOW);
  }
}

// void serialEvent() {
//   while (Serial.available()) {
//     byte inByte = Serial.read();

//     if (inByte == MESSAGE_PATTERN) {

//       while (Serial.available() == 0) { delay(1); }
//       byte targetDevice = Serial.read();
//       if (targetDevice != buttonID) { return; }

//       while (Serial.available() == 0) { delay(1); }
//       byte messageType = Serial.read();

      
//       byte message[NUMPIXELS*3];

//       int bytesRead = Serial.readBytesUntil(';', message, NUMPIXELS*3);  //; is 3B

//       for (int i = 0; i < 36; i++) { Serial.write(message[i]); Serial.flush(); delay(25); }

//       Parse_DRB_Message(message);

//       delay(50);

//       Serial.println("Something else coherent");

//     }
//   }
// }


// void serialEvent() {
//   while (Serial.available()) {
//     byte inByte = Serial.read();

//     if (inByte == MESSAGE_PATTERN) {
//       byte message[MESSAGE_LENGTH];
//       message[0] = inByte;
//       for (short i = 1; i < 37; i++) {
//         message[i] = Serial.read();
//       }
//     }
//   }
// }