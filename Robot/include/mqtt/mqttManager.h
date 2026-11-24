#pragma once
#include "wifi/wifiManager.h"
#include "PubSubClient.h"
#include "api/secrets.h"
#include "locomotion/locomotion.h"
#include "data/status.h"
#include "ArduinoJson.h"

extern PubSubClient mqtt;
extern String clientId;
extern TaskHandle_t taskInitMqttHandler;
extern TaskHandle_t taskPublishDataHandler;


void taskInitMqtt(void *params);
void taskPublishData(void *params);
void taskMqtt(void *params);
bool ensureMqtt();
void cmdProcessor(char* topic, byte* payload, unsigned int len);
void waitForStart();