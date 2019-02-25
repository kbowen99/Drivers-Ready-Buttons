#pragma once
#include "buttonStates.h"
#include "storedSettings.h"
#include "magicMenu.h"

/**
 * Stores the last couple addresses received 
 */
uint8_t addressBuffer[] = {0xFF,0xFE};

/**
 * Converted battery voltage received from each button
 */
float buttonVoltages[] = {5.0F, 5.0F, 5.0F, 5.0F, 5.0F, 0.0F};

/**
 * Last time a message was received from a button
 */
unsigned long buttonLastBroadcast[] = {0, 0, 0, 0, 0, 0};

/**
 * Whether or not each button is believed to be online
 */
bool buttonOnline[] = {false, false, false, false, false, false};

/**
 * returns the number of online buttons
 */
int numberOnlineButtons() {
  int tmp = 0;
  for (int i = 0; i < getNumberDrivers(); i++) {
    tmp += buttonOnline[i];
  }
  return tmp;
}

/**
 * Maps each index to its SA
 */
uint8_t indexToButton(int index) {
  switch (index) {
    case 0:
      return getJudgeSA();
    case 1:
      return getDriver1SA();
    case 2:
      return getDriver2SA();
    case 3:
      return getDriver3SA();
    case 4:
      return getDriver4SA();
    default:
      break;
  }
}

/**
 * Returns a human readable descriptor for each button index
 */
String indexDescriptor(int index) {
  switch (index)
  {
    case 0:
      return "Judge";
    case 1:
    case 2:
    case 3:
    case 4:
      return "Driver " + index;
    default:
      return "Unknown";
  }
}


/**
 * Maps a buttons SA to its index (Judge = 0, Driver 1 = 1, Driver 2 = 2, etc.)
 */
int buttonToIndex(uint8_t SA) {
  for (int i = 0; i < getNumberDrivers(); i++)
    if (SA == indexToButton(i))
      return i;
  return getNumberDrivers();
}

/**
 * handles logic for recording last broadcast time and SA Status
 */
void receivedAddr(int addr) {
    addressBuffer[1] = addressBuffer[0];
    addressBuffer[0] = addr;
    buttonLastBroadcast[buttonToIndex(addr)] = millis();
}

/**
 * Method called in loop to update button states
 */
void updateButtonStates() {
    float lowestVBatt = 5.0F;
    String vBattWarn = "";
    for (int i = 0; i < getNumberDrivers(); i++) {
        if (!buttonOnline[i] && (millis() - buttonLastBroadcast[i]) < BUTTON_LOST_TIMEOUT && buttonLastBroadcast > 0) {
            buttonOnline[i] = true;
            updateMainStatus(indexDescriptor(i) + " online");
            if (numberOnlineButtons() == getNumberDrivers()){
                updateMainStatus("All Buttons Online.");
            }
        } else if (buttonOnline[i] && (millis() - buttonLastBroadcast[i]) > BUTTON_LOST_TIMEOUT) {
            buttonOnline[i] = false;
            updateMainStatus(indexDescriptor(i) + " LOST");
        }
        if (buttonVoltages[i] < LOW_BATTERY_VOLTAGE && buttonVoltages[i] < lowestVBatt) {
            lowestVBatt = buttonVoltages[i];
            vBattWarn = indexDescriptor(i) + String(" ") + String(buttonVoltages[i]) + String("v");
        }
    }
    if (lowestVBatt < 5.0F) {
        updateMainStatus(vBattWarn);
    }
}