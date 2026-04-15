#pragma once 
#include <Arduino.h>

extern const int LED_PIN;

// (gain = 0x0F) gain only defined here
void setWurthLed(bool state, uint16_t r, uint16_t g, uint16_t b, uint8_t gain = 0x0F);
void runEffect(int effectMode);