#include "storedSettings.h"
#include <Arduino.h>
#include <EEPROM.h>

/**
 * EEPROM Address with Flag
 */
#define USE_STORED_CONFIGS_EEPROM               EEPROM.length() - 9

/**
 * EEPROM Address with learned Button addresses.
 */
#define NUM_BUTTONS_EEPROM                      EEPROM.length() - 8

/**
 * Whether or not to require the judge button to operate
 */
#define JUDGE_BUTTON_ENABLED                    EEPROM.length() - 7

/**
 * Learned SA for the Judge Button
 */
#define JUDGE_BUTTON_EEPROM                     EEPROM.length() - 6

/**
 * Learned SA for Driver 1
 */
#define DRIVER_1_BUTTON_EEPROM                  EEPROM.length() - 5

/**
 * Learned SA for Driver 2
 */
#define DRIVER_2_BUTTON_EEPROM                  EEPROM.length() - 4

/**
 * Learned SA for Driver 3
 */
#define DRIVER_3_BUTTON_EEPROM                  EEPROM.length() - 3

/**
 * Learned SA for Driver 4
 */
#define DRIVER_4_BUTTON_EEPROM                  EEPROM.length() - 2

/**
 * Resets all values to default
 */
void resetAll() {
    EEPROM.write(NUM_BUTTONS_EEPROM, 0);
    EEPROM.write(JUDGE_BUTTON_ENABLED, 1);
    EEPROM.write(JUDGE_BUTTON_EEPROM, 0);
    EEPROM.write(DRIVER_1_BUTTON_EEPROM, 0);
    EEPROM.write(DRIVER_2_BUTTON_EEPROM, 0);
    EEPROM.write(DRIVER_3_BUTTON_EEPROM, 0);
    EEPROM.write(DRIVER_4_BUTTON_EEPROM, 0);
    EEPROM.write(USE_STORED_CONFIGS_EEPROM, 0);
}

/**
 * returns whether or not the EEPROM should be used
 */
bool storedSettings() {
    return EEPROM.read(USE_STORED_CONFIGS_EEPROM);
}

/**
 * returns whether or not the judge button should be used
 */
bool getJudgeButton() {
    return EEPROM.read(JUDGE_BUTTON_ENABLED);
}

/**
 * sets whether or not the judge button should be used
 */
void setJudgeButton(bool e) {
    EEPROM.write(JUDGE_BUTTON_ENABLED, e);
    EEPROM.write(USE_STORED_CONFIGS_EEPROM, 1);
}

/**
 * gets the judge button SA
 */
uint8_t getJudgeSA() {
    return EEPROM.read(JUDGE_BUTTON_EEPROM);
}

/**
 * sets the judge button SA
 */
void setJudgeSA(uint8_t sa) {
    EEPROM.write(JUDGE_BUTTON_EEPROM, sa);
}

/**
 * gets the driver 1 button SA
 */
uint8_t getDriver1SA() {
    return EEPROM.read(DRIVER_1_BUTTON_EEPROM);
}

/**
 * sets the driver 1 button SA
 */
void setDriver1SA(uint8_t sa) {
    EEPROM.write(DRIVER_1_BUTTON_EEPROM, sa);
}

/**
 * gets the driver 2 button SA
 */
uint8_t getDriver2SA() {
    return EEPROM.read(DRIVER_2_BUTTON_EEPROM);
}

/**
 * sets the driver 2 button SA
 */
void setDriver2SA(uint8_t sa) {
    EEPROM.write(DRIVER_2_BUTTON_EEPROM, sa);
}

/**
 * gets the driver 3 button SA
 */
uint8_t getDriver3SA() {
    return EEPROM.read(DRIVER_3_BUTTON_EEPROM);
}

/**
 * sets the driver 3 button SA
 */
void setDriver3SA(uint8_t sa) {
    EEPROM.write(DRIVER_3_BUTTON_EEPROM, sa);
}

/**
 * gets the driver 4 button SA
 */
uint8_t getDriver4SA() {
    return EEPROM.read(DRIVER_4_BUTTON_EEPROM);
}

/**
 * sets the driver 4 button SA
 */
void setDriver4SA(uint8_t sa) {
    EEPROM.write(DRIVER_4_BUTTON_EEPROM, sa);
}

/**
 * Returns the number of Learned Drivers Buttons
 */
uint8_t getNumberDrivers() {
    if (!storedSettings())
        return 0;
    return EEPROM.read(NUM_BUTTONS_EEPROM) - EEPROM.read(JUDGE_BUTTON_ENABLED);
}

/**
 * Sets the number of programmed buttons
 */
void setNumberButtons(uint8_t n) {
    EEPROM.write(NUM_BUTTONS_EEPROM, n);
}