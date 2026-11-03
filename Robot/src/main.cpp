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
    server.on("/start", HTTP_POST, startRobot);
    server.on("/stop", HTTP_POST, stopRobot);
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

void startRobot() {
    // Implement the logic to start the robot here
    server.send(200, "text/plain", "Robot started");
}

void stopRobot() {
    // Implement the logic to stop the robot here
    server.send(200, "text/plain", "Robot stopped");
}

void setup() {
  Serial.begin(115200);
  initWebServer();
}

void loop() {
  server.handleClient();
}
