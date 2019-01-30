#pragma once
#include "magicMenu.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include "storedSettings.h"

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
unsigned long btnTimeout = 700;
int tmp1 = 0;
int tmp2 = 0;

uint8_t buttonAddr[2] = {0xFF,0xFE};

String mainStatus = "";

String menuDesc[] = {"Home","Back", "Learn Buttons", "Match Settings", "Reset EEPROM", "Set Color", "Demo Mode", "XBee Viewer", " ", " "};
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
                    //Judge Button
                    switch (lastButtonPress){
                        case LEFT://No
                            tmp1 = 0;
                            break;
                        case RIGHT://Yes
                            tmp1 = 1;
                            break;
                        case DOWN:
                        case SELECT://save
                            subMenu = 2;
                            setJudgeButton(tmp1);
                            tmp1 = 0;
                            break;
                    }
                    break;
                case 2:
                    //No. Drivers
                    switch (lastButtonPress){
                        case LEFT://less
                            tmp1--;
                            break;
                        case RIGHT://more
                            tmp1++;
                            break;
                        case DOWN:
                        case SELECT://save
                            if (tmp1 > 0) {
                                subMenu++;
                                setNumberButtons(tmp1 + getJudgeButton());
                            }
                            break;
                    }
                    break;
                case 3:
                    //Learn Judge, Any key to continue
                    if (tmp2 > 0) {
                        subMenu++;
                        tmp2 = 0;
                    } else
                        tmp2++;
                    break;
                case 4:
                    //verify Judge, Any key to continue
                    if (tmp2 > 0) {
                        tmp2 = 0;
                        if (buttonAddr[0] == buttonAddr[1]){
                            subMenu++;
                            setJudgeSA(buttonAddr[0]);
                            tmp1 = 1;
                        } else 
                            subMenu--;
                    } else
                        tmp2++;
                    break;
                case 5:
                    //set Driver X
                    if (tmp2 > 0) {
                        subMenu++;
                        tmp2 = 0;
                    } else
                        tmp2++;
                case 6:
                    //verify Driver, Any key to continue
                    if (tmp2 > 0) {
                        tmp2 = 0;
                        if (buttonAddr[0] == buttonAddr[1]){
                            if (tmp1 < getNumberDrivers()){
                                tmp1++;
                                subMenu=5;
                            } else {
                                subMenu=7;
                                tmp1 = 0;
                            }
                        } else 
                            subMenu--;
                    } else {
                        tmp2++;
                    }
                    break;
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
                case 1://judge mode enable
                    lcd.setCursor(0,0);
                    lcd.print("Use Judge Button?");
                    lcd.setCursor(0,1);
                    lcd.print((tmp1 ? "Yes" : "No"));
                    lcd.print("                    ");
                    break;
                case 2://number driver buttons
                    lcd.setCursor(0,0);
                    lcd.print("No. Drivers:    ");
                    lcd.setCursor(0,1);
                    lcd.print(tmp1);
                    lcd.print("                    ");
                    break;
                case 3://learn Judge
                    lcd.setCursor(0,0);
                    lcd.print("Press Judge     ");
                    lcd.setCursor(0,1);
                    lcd.print("Button 2x       ");
                    break;
                case 4://verify Judge
                    lcd.setCursor(0,0);
                    lcd.print("Judge ");
                    lcd.print(buttonAddr[0] == buttonAddr[1] ? "Set" : "Not Set");
                    lcd.print("           ");
                    lcd.setCursor(0,1);
                    lcd.print("Addr:");
                    if (buttonAddr[0] == buttonAddr[1])
                        lcd.print(buttonAddr[0]);
                    lcd.print("                ");
                    break;
                case 5://Learn Driver X
                    lcd.setCursor(0,0);
                    lcd.print("Press Driver ");
                    lcd.print(tmp1);
                    lcd.print("          ");
                    lcd.setCursor(0,1);
                    lcd.print("Button 2x       ");
                case 6://verify Driver X
                    lcd.setCursor(0,0);
                    lcd.print("Driver ");
                    lcd.print(tmp1);
                    lcd.print(buttonAddr[0] == buttonAddr[1] ? " Set" : " Not Set");
                    lcd.print("           ");
                    lcd.setCursor(0,1);
                    lcd.print("Addr:");
                    if (buttonAddr[0] == buttonAddr[1])
                        lcd.print(buttonAddr[0]);
                    lcd.print("                ");
                    break;
                case 7:
                    lcd.setCursor(0,0);
                    lcd.print("All Buttons Set");
                    lcd.setCursor(0,1);
                    lcd.print("Saved to EEPROM");
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

void receivedAddr(uint8_t addr){
    buttonAddr[1] = buttonAddr[0];
    buttonAddr[0] = addr;
}