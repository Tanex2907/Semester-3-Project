#include <Arduino.h>
#include "wifi/wifiManager.h"
#include "mqtt/mqttManager.h"
#include "sensors/temperature.h"
#include "data/status.h"
#include "sensors/battery.h"
#include "sensors/capacity.h"


#define NETWORK_CORE      0
#define APP_CORE          1

StatusData data = {0,0,0};

void setup()
{
  Serial.begin(115200);
  //Event Groups
  eventGroupStartEvent = xEventGroupCreate();

  //Networking tasks
  xTaskCreatePinnedToCore(taskInitMqtt, "INIT MQTT", 4096, NULL, 1, &taskInitMqttHandler, NETWORK_CORE);
  xTaskCreatePinnedToCore(taskInitWifi, "INIT WIFI", 4096, NULL, 1, &taskInitWifiHandler, NETWORK_CORE);
  xTaskCreatePinnedToCore(taskPublishData, "PUB DATA", 3096, &data, 3, &taskPublishDataHandler, NETWORK_CORE);
  xTaskCreatePinnedToCore(taskMqtt, "Timer for mqttt", 2048, NULL, 2, NULL, NETWORK_CORE);
  //App tasks
  xTaskCreatePinnedToCore(taskReadTemperature, "TEMP READ", 1048, &data, 5, &taskReadTemperatureHandler, APP_CORE);
  xTaskCreatePinnedToCore(taskReadBattery, "TEMP READ", 1048, &data, 5, &taskReadBatteryHandler, APP_CORE);
  xTaskCreatePinnedToCore(taskReadCapacity, "TEMP READ", 1048, &data, 5, &taskReadCapacityHandler, APP_CORE);
}

void loop()
{
  //Do nothing here
}

