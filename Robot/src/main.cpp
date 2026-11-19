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

int startmillis;
int currentmillis;
int ledPin = 17;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(provisionManager.generateID(), USER, PWD)) {
      Serial.println("connected");
      client.subscribe("cmd/start");
      client.subscribe("cmd/stop");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  if (String(topic) == "cmd/start") {
    Serial.print("Changing output to high");
    digitalWrite(ledPin, HIGH);
  }
  if(String(topic) == "cmd/stop")
  {
    Serial.print("Changing output to low");
    digitalWrite(ledPin, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  provisionManager.initWiFi();
  startmillis = millis();
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }
  currentmillis = millis();

  if (currentmillis - startmillis > 1000) {
    livedataDto snapshot = livedata.toDto();
    JsonDocument doc;
    doc["battery"] = random(0,100);
    doc["temperature"] = random(0,100);
    doc["fullness"] = random(0,100);

    char buffer[256];

    serializeJson(doc, buffer);
    client.publish("telemetry/livedata", buffer, true);
    startmillis = currentmillis;
    Serial.println("send livedata");
  }

}