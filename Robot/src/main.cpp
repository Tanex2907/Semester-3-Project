#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <web/webserver.h>

const char* ssid = "fake-ssid";
const char* password = "fake-password";

WebServer server(PORT);

void initWebServer() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    routes();

    server.begin();
}

void routes() {
    server.on("/livedata", HTTP_GET, getLiveData);
}

void handleClient() {
    server.handleClient();
}

void getLiveData() {
    Livedata liveData(75, 50, 22);
    livedataDto dto = Livedata::toDto(liveData);

    JsonDocument jsonDoc;
    jsonDoc["battery"] = dto.battery;
    jsonDoc["fullness"] = dto.fullness;
    jsonDoc["temperature"] = dto.temperature;

    String response;
    serializeJson(jsonDoc, response);

    server.send(200, "application/json", response);
}


void setup() {
  Serial.begin(115200);
  initWebServer();
}

void loop() {
  server.handleClient();
}
