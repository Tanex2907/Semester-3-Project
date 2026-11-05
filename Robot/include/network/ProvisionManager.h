#pragma once
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

typedef struct {
    char ssid[32];
    char password[64];
} Credentials;


#define MDNS_HOSTNAME "robot"




const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_SUBNET(255, 255, 255, 0);

class ProvisionManager {
private:
    Preferences preferences;
    Credentials credentials;
    WebServer& server;
    DNSServer dnsServer;

    const char* AP_SSID = "Robot";
    const char* AP_PASSWORD = "configure";
public:
    ProvisionManager(WebServer& server);
    void initWiFi();
    void startProvisioning();
    void handleRoot();
    void handleSaveCredentials();
    void clearCredentials();
    void saveCredentials();
    bool loadCredentials();
};

