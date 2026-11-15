#include <Arduino.h>
#include "LiveData/livedata.h"
#include "network/ProvisionManager.h"
#include "network/WebController.h"

Livedata livedata;
WebServer server(80);
WebController webController(livedata, server);
ProvisionManager provisionManager(server);

void setup() {
  Serial.begin(115200);

  provisionManager.initWiFi();
  webController.init();
}

void loop() {

  webController.handleClient();
}
