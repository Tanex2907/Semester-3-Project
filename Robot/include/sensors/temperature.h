#pragma once
#include "data/status.h"
#include "mqtt/mqttManager.h"

extern TaskHandle_t taskReadTemperatureHandler;
#define ADC_VREF_mV         3300
#define ADC_RESOLUTION      4096
#define PIN_LM35            35

void taskReadTemperature(void *params);