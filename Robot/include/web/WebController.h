#pragma once
#include <WebServer.h>
#include <WiFi.h>
#include "LiveData/Livedata.h"
#include <ArduinoJson.h>

class WebController {
private:
    WebServer server;
    Livedata& data;

    const char* ssid;
    const char* password;


public:
    WebController(Livedata& sharedData,const char* s, const char* p, uint16_t port = 80);
    void init();
    void handleClient();
    void handleLiveData(Livedata& livedata);
    void handleStartRobot();
    void handleStopRobot();
};