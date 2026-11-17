#include <Arduino.h>
#include "LiveData/Livedata.h"
#include "network/ProvisionManager.h"
#include "network/WebController.h"
#include <PubSubClient.h>
#include "keys.h"

Livedata livedata;
WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);
ProvisionManager provisionManager(server, client);

int sendFlag = 0;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(provisionManager.generateID(), CHANNEL, "")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  provisionManager.initWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }

  if (sendFlag == 0 && client.connected()) {
    livedataDto snapshot = livedata.toDto();
    JsonDocument doc;
    doc["battery"] = snapshot.battery;
    doc["temperature"] = snapshot.temperature;
    doc["fullness"] = snapshot.fullness;

    char buffer[256];

    client.publish("app/livedata", buffer);
    serializeJson(doc, buffer);

    sendFlag = 1;
    Serial.println("send livedata");
  }
}