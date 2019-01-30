#include "storedSettings.h"
#include <Arduino.h>
#include <EEPROM.h>

/**
 * EEPROM Address with Flag
 */
#define USE_STORED_CONFIGS_EEPROM               EEPROM.length() - 9

/**
 * Learned Team Color 'B'
 */
#define TEAM_COLOR_B                  EEPROM.length() - 4

/**
 * Learned Team Color 'G'
 */
#define TEAM_COLOR_G                  EEPROM.length() - 3

/**
 * Learned Team Color 'R'
 */
#define TEAM_COLOR_R                  EEPROM.length() - 2

/**
 * Resets all values to default
 */
void resetAll() {
    EEPROM.write(TEAM_COLOR_R, 255);
    EEPROM.write(TEAM_COLOR_G, 255);
    EEPROM.write(TEAM_COLOR_B, 255);
    EEPROM.write(USE_STORED_CONFIGS_EEPROM, 1);
}

/**
 * returns whether or not the EEPROM should be used
 */
bool useStoredSettings() {
    return EEPROM.read(USE_STORED_CONFIGS_EEPROM);
}

/**
 * Sets whether or not to use stored team color
 */
void setUseStored(bool b) {
    EEPROM.write(USE_STORED_CONFIGS_EEPROM, b);
}

/**
 * Sets R Value for Team Color
 */
void setColorR(int_fast8_t r) {
    EEPROM.write(TEAM_COLOR_R, r);
}

/**
 * Sets G Value for Team Color
 */
void setColorG(int_fast8_t c) {
    EEPROM.write(TEAM_COLOR_G, c);
}

/**
 * Sets B Value for Team Color
 */
void setColorB(int_fast8_t c) {
    EEPROM.write(TEAM_COLOR_B, c);
}

/**
 * gets R Value for Team Color
 */
uint8_t getColorR() {
    return EEPROM.read(TEAM_COLOR_R);
}

/**
 * gets G Value for Team Color
 */
uint8_t getColorG() {
    return EEPROM.read(TEAM_COLOR_G);
}

/**
 * gets B Value for Team Color
 */
uint8_t getColorB() {
    return EEPROM.read(TEAM_COLOR_B);
}