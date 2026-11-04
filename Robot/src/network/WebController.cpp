#include "network/WebController.h"

WebController::WebController(Livedata& sharedData,const char* s, const char* p, uint16_t port)
    : server(port), ssid(s), password(p), data(sharedData) {}

void WebController::init() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

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
}

//TODO: Implement the logic to start and stop the robot in these methods
void WebController::handleStartRobot() {
    // Implement the logic to start the robot here
    server.send(200, "text/plain", "Robot started");
}


//TODO: Implement the logic to start and stop the robot in these methods
void WebController::handleStopRobot() {
    // Implement the logic to stop the robot here
    server.send(200, "text/plain", "Robot stopped");
}



