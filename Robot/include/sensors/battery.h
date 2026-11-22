#pragma once

#include "data/status.h"
#include "mqtt/mqttManager.h"

extern TaskHandle_t taskReadBatteryHandler;

void taskReadBattery(void *params);