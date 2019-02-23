#include <Arduino.h>
#include "XBeeProto.h"
#include "neoDraw.h"
#include "main.h"
#include "storedSettings.h"

#define BUTTON_PIN 3
#define BUTTON_TIMEOUT 500
#define BUTTON_HOLD_DURATION 4000.0F
#define DEBUG_OUTPUT FALSE
#define BATTERY_VOLTAGE_PIN A0
#define MAX_BATTERY_VOLTAGE 4.2
#define MIN_BATTERY_VOLTAGE 3.0
#define LOW_BATTERY_VOLTAGE 3.2
#define MAX_ARDUINO_ADC 1023
#define MAX_ARDUINO_VOLTAGE 5.0
#define BATTERY_BROADCAST_INTERVAL 10 * 1000

uint32_t currentImage[NUMPIXELS];
bool imageChanged = false;

buttonState currentState = IDLE;
buttonState lastState = IDLE;

unsigned long lastButtonSmash = 0;
unsigned long lastBatteryUpdate = 0;
bool lastButtonSmashState = false;
bool lastHold = false;

void buttonSmash()
{
  switch (currentState)
  {
  case IDLE:
  case ARMED:
    if ((millis() - lastButtonSmash) > BUTTON_TIMEOUT && !lastHold)
    {
      lastButtonSmash = millis();
      send_Button_Pressed();
      setSpiralColor(getColorR(), getColorG(), getColorB());
      setSpiralColor(0, 0, 0);
    }
    break;
  case GAME:
    break;
  case GAMEOVER:
  default:
    break;
  }

  if (lastHold)
  {
    setColor(0, 0, 0);
    lastHold = false;
  }

  lastButtonSmashState = digitalRead(BUTTON_PIN);
}

void setup()
{
  //setColorR(0);setColorG(255);setColorB(0);setUseStored(true);setButtonID(0x2);
  setupNeopixel();
  setSpiralColor(getColorR(), getColorG(), getColorB());
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonSmash, CHANGE);

  //rainbowCycle(2);
  setSpiralColor(0, 0, 0);
}

void loop()
{
  //draw();
  updateVbatt();

  switch (currentState)
  {
  case IDLE:
    /* code */
    break;
  case ARMED:
    /* code */
    break;
  case GAME:
    if (!lastButtonSmashState && !digitalRead(BUTTON_PIN) && ((millis() - lastButtonSmash) > BUTTON_TIMEOUT))
    {
      if ((millis() - lastButtonSmash) < BUTTON_HOLD_DURATION)
      {
        float delta = millis() - lastButtonSmash;
        setSpiralPercentage(delta / BUTTON_HOLD_DURATION);
        lastHold = true;
      }
      else
      {
        setColor(255, 0, 0);
        send_button_gameOver();
      }
      delay(10);
    }
    break;
  case GAMEOVER:
    /* code */
    break;

  default:
    break;
  }
}

/**
 * Serial Event interrupt
 */
void serialEvent()
{
  readSerial();
}

/**
 * Maps the current battery voltage from MIN_ARDUINO_VOLTAGE to MAX_ARDUINO_VOLTAGE --> 0 to 255
 */
void updateVbatt()
{
  if ((millis() - lastBatteryUpdate) > BATTERY_BROADCAST_INTERVAL)
  {
    send_battery_status(map(analogRead(BATTERY_VOLTAGE_PIN), 0, MAX_ARDUINO_ADC, 0x00, 0xFF));
    lastBatteryUpdate = millis();
  }
}
