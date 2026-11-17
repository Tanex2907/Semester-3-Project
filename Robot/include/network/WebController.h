#pragma once
#include <WebServer.h>
#include <WiFi.h>
#include "LiveData/Livedata.h"
#include <ArduinoJson.h>

class WebController {
private:
    WebServer& server;
    Livedata& data;


public:
    WebController(Livedata& sharedData, WebServer& webServer);
    void init();
    void handleClient();
    void handleLiveData(Livedata& livedata);
    void handleStartRobot();
    void handleStopRobot();
};