#pragma once
#include "data/status.h"
#include "mqtt/mqttManager.h"

extern TaskHandle_t taskReadTemperatureHandler;

void taskReadTemperature(void *params);