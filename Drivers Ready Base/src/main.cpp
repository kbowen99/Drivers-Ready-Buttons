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
}

void loop() {
  //uint8_t buttons = lcd.readButtons();

  if (Serial1.available() >= 37) {
    byte inByte = Serial1.read();

    if (inByte == 0xB7) {
      byte message[39];
      message[0] = inByte;
      for (short i = 1; i < 39; i++) {
        if (Serial1.available()) {  message[i] = Serial1.read(); }
      }
      
      Serial.print("-------\n");
      for (int i = 0; i < 39; i++) {
        Serial.println(message[i], HEX);
      }
      Serial.print("-------\n");
    }
    //lcd.clear();
    //lcd.print(Serial1.read());
    //Serial.print(Serial1.read(), HEX);
  }
}

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