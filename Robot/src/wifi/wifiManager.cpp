#include "wifi/wifiManager.h"

WiFiClient client;
TaskHandle_t taskInitWifiHandler = NULL;
EventGroupHandle_t eventGroupStartEvent;

void taskInitWifi(void *params)
{
  while (!ensureWifi())
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  Serial.println("Connected to WIFI");
  xEventGroupSetBits(eventGroupStartEvent, BITWIFI);
  vTaskDelete(NULL);
}

bool ensureWifi() {
  if (WiFi.status() == WL_CONNECTED) return true;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  for (int i = 0; i < 40 && WiFi.status() != WL_CONNECTED; i++) {
    vTaskDelay(pdMS_TO_TICKS(250));
  }
  return WiFi.status() == WL_CONNECTED;
}