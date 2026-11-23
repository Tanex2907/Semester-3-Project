#pragma once
#include "Arduino.h"

#define BITSTART            BIT0
#define BITHIT              BIT1

extern EventGroupHandle_t eventGroupLocomotionEvent;

extern TaskHandle_t taskForwardHandler;
extern TaskHandle_t taskBackwardHandler;

void taskForward(void *params);
void taskBackward(void *params);