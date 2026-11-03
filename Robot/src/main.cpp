#include <Arduino.h>
#include "LiveData/livedata.h"
#include "web/WebController.h"

Livedata livedata;
WebController webController("MY_SSID", "MY_PASSWORD");

void setup() {
  Serial.begin(115200);
  webController.init();
}

void loop() {
  webController.handleClient();
}
