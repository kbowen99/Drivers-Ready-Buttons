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
  if (Serial1.available() > 0) {
    lcd.clear();
    //String test = Serial1.read();
    //Serial.println(Serial1.read());
    lcd.print(Serial1.read());
  }
}