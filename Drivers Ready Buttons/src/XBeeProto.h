#pragma once
#include "neoDraw.h"

void send_Button_Pressed();
void send_button_gameOver();
void Parse_DRB_Message(byte message[]);
void readSerial();
byte checkSum(byte message[], int numBytes);
void send_serial_spaced(byte m[], int l);
