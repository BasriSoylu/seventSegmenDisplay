#ifndef __SEVENSEGMENT__
#define __SEVENSEGMENT__

#include <Arduino.h>

void sevenSegmentInit(byte clkPin, byte dataPin);
void seventSegmentShowDigit(char *pString);
void seventSegmentSetBrightness(byte value);

#endif