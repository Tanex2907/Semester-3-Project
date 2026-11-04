#pragma once
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

#define MDNS_HOSTNAME "robot"

WebServer server(80);
DNSServer dnsServer;

struct Credentials {
    char ssid[32];
    char password[64];
};

const char* AP_SSID     = "Robot";
const char* AP_PASSWORD = "configure"; 

const IPAddress AP_IP(192, 168, 4, 1);
const IPAddress AP_SUBNET(255, 255, 255, 0);

Preferences preferences;
Credentials credentials;

bool loadCredentials();
void saveCredentials();
void clearCredentials();

void handleSaveCredentials();
void startProvisioning();
