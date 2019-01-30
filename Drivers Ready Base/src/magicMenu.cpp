#pragma once
#include "magicMenu.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

#define PROGRAM_NAME            "ROBOCONTROL"
#define SCREEN_WIDTH            16
#define SCREEN_LENGTH           2

//Button Menu Input
enum menuDir {NONE,UP,DOWN,LEFT,RIGHT,SELECT};

//Variables
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
bool screenChanged = true;
bool updateNavigation = false;
bool continuousDrawMenu = false;
int currMainMenu = 0;
int lastDrawnMainMenu = -1;
int subMenu = 1;
menuDir lastButtonPress = menuDir::NONE;
unsigned long lastMenuBtnPress = 0;
unsigned long menuTimeout = 15*1000;
unsigned long btnTimeout = 500;
int tmp1 = 0;
int tmp2 = 0;

String mainStatus = "";

String menuDesc[] = {"Home","Back", "Learn Buttons", "Match Settings", "Set Color", "Demo Mode", "XBee Viewer", " ", " "};
int menuLength = 7;
/**
 * initializes the LCD, displays startup text
 */
void setupLCD(){
    lcd.begin(16, 2);
    lcd.setBacklight(0x1);
    lcd.setCursor(0,0); 
}

/**
 * Updates LCD/Menu Navigation, should be called in main loop()
 */
void updateLCD(){
    readButtons();
    if (updateNavigation)
        navigateMenus();
    if (screenChanged || continuousDrawMenu)
        drawCurrent();
    if (currMainMenu != 0 && (millis() - lastMenuBtnPress > menuTimeout)){
        currMainMenu = 0;
        screenChanged = true;
    }
}

/**
 * Grabs the current button status from the LCD
 */
void readButtons(){
    uint8_t buttons = lcd.readButtons();

    switch(buttons){
        case BUTTON_LEFT:
            lastButtonPress = menuDir::LEFT;
            break;
        case BUTTON_RIGHT:
            lastButtonPress = menuDir::RIGHT;
            break;
        case BUTTON_DOWN:
            lastButtonPress = menuDir::DOWN;
            break;
        case BUTTON_UP:
            lastButtonPress = menuDir::UP;
            break;
        case BUTTON_SELECT:
            lastButtonPress = menuDir::SELECT;
            break;
        default:
            break;
    }
    if (buttons && (millis() - lastMenuBtnPress > btnTimeout)) {
        lastMenuBtnPress = millis();
        updateNavigation = true;
    }
}

/**
 * Performs logic necessary for progression through menus
 */
void navigateMenus(){
    screenChanged = true;
    switch (currMainMenu)
    {
        case 2:
            //Learn Buttons
            switch (subMenu)
            {
                case 1:
                    switch (lastButtonPress){
                        case LEFT:
                            tmp1 = 0;
                            break;
                        case RIGHT:
                            tmp1 = 1;
                            break;
                        case DOWN:
                        case SELECT:
                            subMenu++;
                    }
            
                default:
                    break;
            }
            break;
        case 1:
            //Main Menu
            switch (lastButtonPress){
                case UP:
                    if (subMenu > 1)
                        subMenu = subMenu - 1;
                    break;
                case DOWN:
                    if (subMenu < menuLength)
                        subMenu = subMenu + 1;
                    break;
                case LEFT:
                    subMenu = 1;
                    currMainMenu = 0;
                    break;
                case RIGHT:
                case SELECT:
                    if (currMainMenu == subMenu)
                        currMainMenu = 0;
                    else
                        currMainMenu = subMenu;
                        subMenu = 1;
                    break;
            }
            break;
        case 0:
            //0 = Home, any button should open main menu "list"
            currMainMenu = 1;
            break;
        case -1:
            //-1 = Not Found, any button should return home
            currMainMenu = 0;
            break;
        default:
            currMainMenu = -1;
            subMenu = 1;
            break;
    }
    updateNavigation = false;
}

/**
 * Draws the current Menu onto the LCD
 */
void drawCurrent(){
    if (lastDrawnMainMenu != currMainMenu)
        lcd.clear();
    continuousDrawMenu = false;
    switch (currMainMenu)
    {    
        case 0: //"Home"
            if (lastDrawnMainMenu != currMainMenu){
                lcd.print(PROGRAM_NAME);
                lcd.println("     ");
            }
            lcd.setCursor(0, 1);
            lcd.println(mainStatus);
            break;
        case 1://"Main Menu"
            for (int i = 0; i < 2; i++){
                lcd.setCursor(0,i);
                if (i == 0)
                    lcd.print("*");
                else
                    lcd.print(" ");
                lcd.print(menuDesc[subMenu + i]);
                lcd.print("                         ");
            }
            break;
        case 2://Learn Menu
            //s = stage
            switch (subMenu) {
                case 1:
                    lcd.setCursor(0,0);
                    lcd.print("Use Judge Button");
                    lcd.setCursor(0,1);
                    lcd.print((tmp1 ? "Yes" : "No"));
                    lcd.print("                    ");
                    break;
            }
            break;
        case -1:
        default://Not Found timeout
            if (lastDrawnMainMenu != currMainMenu){
                lcd.println("Not Found      ");
                lcd.setCursor(0, 1);
                lcd.print("Timeout in:");
            }
            lcd.setCursor(11, 1);
            lcd.print(((lastMenuBtnPress + menuTimeout) - millis())/1000);
            lcd.print("s");
            continuousDrawMenu = true;
            break;
            break;
    }
    screenChanged = false;
    lastDrawnMainMenu = currMainMenu;
}

/**
 * Method to put info on the main screen (IE: Running, waiting, etc.)
 */
void updateMainStatus(String s){
    mainStatus = s + "           ";
    screenChanged = true;
    updateLCD();
}
