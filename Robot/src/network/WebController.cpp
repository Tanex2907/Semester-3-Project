#include "network/WebController.h"

WebController::WebController(Livedata& sharedData, WebServer& webServer) : server(webServer),
    data(sharedData) {}

void WebController::init() {


    server.on("/", [this]() {
        server.send(200, "text/plain", "Hello from ESP32!");
        Serial.println("Request received at /");
    });
    server.on("/livedata", HTTP_GET, [this]() {
        handleLiveData(data);
    });
    server.on("/start", HTTP_POST, [this]() {
        handleStartRobot();
    });
    server.on("/stop", HTTP_POST, [this]() {
        handleStopRobot();
    });

    server.begin();
}

void WebController::handleClient() {
    server.handleClient();
}

void WebController::handleLiveData(Livedata& livedata) {
    livedataDto snapshot = livedata.toDto();
    JsonDocument doc;
    doc["battery"] = snapshot.battery;
    doc["fullness"] = snapshot.fullness;
    doc["temperature"] = snapshot.temperature;

    String response;
    serializeJson(doc, response);

    server.send(200, "application/json", response);
    Serial.println("Live data sent: " + response);
}

//TODO: Implement the logic to start and stop the robot in these methods
void WebController::handleStartRobot() {
    server.send(200, "text/plain", "Robot started");
    Serial.println("Robot started");
}

//TODO: Implement the logic to start and stop the robot in these methods
void WebController::handleStopRobot() {
    server.send(200, "text/plain", "Robot stopped");
    Serial.println("Robot stopped");
}



