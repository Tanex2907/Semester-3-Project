#include "network/ProvisionManager.h"

const char* PROVISION_HTML = R"(
	<!DOCTYPE html>
	<html>
	<head>
	  <meta charset="utf-8">
	  <meta name="viewport" content="width=device-width, initial-scale=1">
	  <title>WiFi Setup</title>
	  <style>
	    body { font-family: sans-serif; max-width: 400px; margin: 60px auto; padding: 0 20px; }
	    input { width: 100%; padding: 8px; margin: 8px 0; box-sizing: border-box; }
	    button { width: 100%; padding: 10px; background: #0078d4; color: white; border: none; cursor: pointer; }
	  </style>
	</head>
	<body>
	  <h2>WiFi Setup</h2>
	  <form action="/save" method="POST">
	    <label>SSID</label>
	    <input type="text" name="ssid" required>
	    <label>Password</label>
	    <input type="password" name="password">
	    <button type="submit">Save and Reboot</button>
	  </form>
	</body>
	</html>
)";

ProvisionManager::ProvisionManager(WebServer& server) : server(server) {}

bool ProvisionManager::loadCredentials() {
	preferences.begin("wifi", true);
	bool exists = preferences.getBytesLength("creds") == sizeof(credentials);
	if (exists) {
	    preferences.getBytes("creds", &credentials, sizeof(credentials));
	}
	preferences.end();
	return exists;
}

void ProvisionManager::saveCredentials() {
    preferences.begin("wifi", false);
    preferences.putBytes("creds", &credentials, sizeof(credentials));
    preferences.end();
}

void ProvisionManager::clearCredentials() {
    preferences.begin("wifi", false);
    preferences.clear();
    preferences.end();
}

void ProvisionManager::handleRoot() {
    server.send(200, "text/html", PROVISION_HTML);
}

void ProvisionManager::handleSaveCredentials() {

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

void ProvisionManager::startProvisioning() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    WiFi.softAPConfig(AP_IP, AP_IP, AP_SUBNET);

    dnsServer.start(53, "*", AP_IP);

    server.on("/", [this]() { handleRoot(); });
    server.on("/save", [this]() { handleSaveCredentials(); });
    server.onNotFound([this]() { handleRoot(); });
    server.begin();

    while (true) {
        dnsServer.processNextRequest();
        server.handleClient();
    }
}

void ProvisionManager::initWiFi() {
  if (!loadCredentials()) {
    Serial.println("No WiFi credentials found. Starting provisioning...");
    startProvisioning();
  }

  Serial.print("Connecting to WiFi SSID: ");
  Serial.println(credentials.ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(credentials.ssid, credentials.password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin(MDNS_HOSTNAME)) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
  }

  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS responder started. Access your ESP32 at http://" + String(MDNS_HOSTNAME) + ".local");
}