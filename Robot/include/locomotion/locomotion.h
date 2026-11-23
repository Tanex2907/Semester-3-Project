#pragma once
#include "Arduino.h"

#define BITSTART           BIT0

extern TaskHandle_t taskForwardHandler;
extern EventGroupHandle_t eventGroupLocomotionEvent;

void taskForward(void *params);