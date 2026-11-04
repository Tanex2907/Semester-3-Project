#include "network/ProvisionManager.h"

bool loadCredentials() {
	preferences.begin("wifi", true);
	bool exists = preferences.getBytesLength("creds") == sizeof(credentials);
	if (exists) {
	    preferences.getBytes("creds", &credentials, sizeof(credentials));
	}
	preferences.end();
	return exists;
}

void saveCredentials() {
    preferences.begin("wifi", false);
    preferences.putBytes("creds", &credentials, sizeof(credentials));
    preferences.end();
}

void clearCredentials() {
    preferences.begin("wifi", false);
    preferences.clear();
    preferences.end();
}

void handleSaveCredentials() {

	if (!server.hasArg("ssid") || server.arg("ssid").isEmpty()) {
	    server.send(400, "text/plain", "SSID required");
	    return;
	}
	 
	server.arg("ssid").toCharArray(credentials.ssid, sizeof(credentials.ssid));
	server.arg("password").toCharArray(credentials.password, sizeof(credentials.password));
	 
	saveCredentials();
	 
	server.send(200, "text/plain", "Saved. Rebooting...");
	server.handleClient();
	delay(1000);
	ESP.restart();
}

void startProvisioning() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    WiFi.softAPConfig(AP_IP, AP_IP, AP_SUBNET);

    dnsServer.start(53, "*", AP_IP);

    server.on("/save", handleSaveCredentials);
    server.onNotFound([]() {
        server.send(200, "text/plain", "Provisioning page. Use /save to save credentials.");
    });
    server.begin();

    while (true) {
        dnsServer.processNextRequest();
        server.handleClient();
    }
}