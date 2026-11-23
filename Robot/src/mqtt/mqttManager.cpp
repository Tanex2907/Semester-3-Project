#include "mqtt/mqttManager.h"
#include "api/secrets.h"
#include "locomotion/locomotion.h"

PubSubClient mqtt(client);
String clientId = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
TaskHandle_t taskInitMqttHandler = NULL;
TaskHandle_t taskPublishDataHandler = NULL;

void taskInitMqtt(void *params)
{
  //do NOT clear the WiFi bit when the wait returns
  xEventGroupWaitBits(eventGroupStartEvent, BITWIFI, pdFALSE, pdTRUE, portMAX_DELAY);

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(cmdProcessor);
  mqtt.setKeepAlive(5);

  while (!ensureMqtt())
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  Serial.println("Connected to MQTT");

  mqtt.subscribe("cmd/start");
  mqtt.subscribe("cmd/stop");

  xEventGroupSetBits(eventGroupStartEvent, BITMQTT);
  vTaskDelete(NULL);
}

bool ensureMqtt() {
  if (mqtt.connected()) return true;
  const char* willTopic = "/status";
  bool ok = mqtt.connect(clientId.c_str(),MQTT_USER, MQTT_PWD);
  if (ok) {
    mqtt.publish(willTopic, "online", true);
  }
  return ok;
}

void cmdProcessor(char* topic, byte* payload, unsigned int len)
{
  if (strcmp(topic, "cmd/start") == 0)
  {
    xEventGroupSetBits(eventGroupLocomotionEvent, BITSTART);
  }
  else if (strcmp(topic, "cmd/stop") == 0)
  {
    xEventGroupClearBits(eventGroupLocomotionEvent, BITSTART);
  }
}

void taskMqtt(void *params)
{
  xEventGroupWaitBits(eventGroupStartEvent, BITWIFI | BITMQTT, pdFALSE, pdTRUE, portMAX_DELAY);
  for (;;)
  {
    if (!mqtt.connected())
    {
      xEventGroupClearBits(eventGroupStartEvent, BITMQTT);
      if (ensureMqtt())
      {
        mqtt.subscribe("cmd/start");
        mqtt.subscribe("cmd/stop");
        xEventGroupSetBits(eventGroupStartEvent, BITMQTT);
      }
      else
      {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }
    mqtt.loop();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void waitForStart() {
  xEventGroupWaitBits(
    eventGroupStartEvent,
    BITWIFI | BITMQTT,   // bits to wait for
    pdFALSE,               // don't clear them on exit (other tasks need them too)
    pdTRUE,                // wait for ALL bits, not just one
    portMAX_DELAY          // wait forever
  );
}

void taskPublishData(void *params){
  waitForStart();

    StatusData *data = (StatusData*) params;
  while (1)
  {
    JsonDocument doc;
    doc["battery"] = data->battery;
    doc["temperature"] = data->temp;
    doc["fullness"] = data->fullness;

    char buffer[256];

    serializeJson(doc, buffer);
    mqtt.publish("telemetry/livedata", buffer, true);

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}