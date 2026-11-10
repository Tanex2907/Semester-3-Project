#pragma once
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

typedef struct {
    char ssid[32];
    char password[64];
} Credentials;

#define MDNS_HOSTNAME "robot"

class ProvisionManager {
private:
    Preferences preferences;
    Credentials credentials;
    WebServer& server;
    DNSServer dnsServer;
    PubSubClient& client;
    WiFiClient wifiClient;
    const char chars[63] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    char id[17];

    const char* AP_SSID = "Robot";
    const char* AP_PASSWORD = "configure";
public:
    ProvisionManager(WebServer& server, PubSubClient& client);
    void initWiFi();
    void startProvisioning();
    void handleRoot();
    void handleSaveCredentials();
    void clearCredentials();
    void saveCredentials();
    bool loadCredentials();
    const char * generateID();
};