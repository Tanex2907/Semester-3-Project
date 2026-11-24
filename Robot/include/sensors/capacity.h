#pragma once

#include "data/status.h"
#include "mqtt/mqttManager.h"

extern TaskHandle_t taskReadCapacityHandler;

void taskReadCapacity(void *params);

