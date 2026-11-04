#include <Arduino.h>
#include "LiveData/livedata.h"
#include "network/ProvisionManager.h"

Livedata livedata;

void setup() {
  Serial.begin(115200);

  if (!loadCredentials()) {
    Serial.println("No WiFi credentials found. Starting provisioning...");
    startProvisioning();
  }

  Serial.print("Connecting to WiFi SSID: ");
  Serial.println(credentials.ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(credentials.ssid, credentials.password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(MDNS_HOSTNAME)) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
  }

  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS responder started. Access your ESP32 at http://" + String(MDNS_HOSTNAME) + ".local");

}

void loop() {
}
