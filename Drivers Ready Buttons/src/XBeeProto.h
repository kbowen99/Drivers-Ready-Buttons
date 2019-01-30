#pragma once
#include "neoDraw.h"

void send_Button_Pressed();
void Parse_DRB_Message(byte message[]);
void readSerial();
byte checkSum(byte message[], int numBytes);
