#pragma once
#include "WiFi.h"
#include "Arduino.h"
#include "api/secrets.h"


#define BITWIFI           BIT0
#define BITMQTT           BIT1

extern WiFiClient client;
extern TaskHandle_t taskInitWifiHandler;
extern EventGroupHandle_t eventGroupStartEvent;

bool ensureWifi();

void taskInitWifi(void *params);

