#ifndef RADIO_WEB_SERVER_H
#define RADIO_WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>

class StationManager;

class RadioWebServer {
public:
    void begin(StationManager* stations, bool apMode = false);
    void handleClient();

private:
    WiFiServer _server{80};
    StationManager* _stations = nullptr;
    bool _apMode = false;

    void handleRoot(WiFiClient& client);
    void handleStations(WiFiClient& client);
    void handleSave(WiFiClient& client, const String& body);
    void handleApiStatus(WiFiClient& client);
    void handleApiWifi(WiFiClient& client, const String& body);
    void handleApiWifiScan(WiFiClient& client);
    void handleNotFound(WiFiClient& client);
    void send200(WiFiClient& client, const char* type, const String& body);
};

#endif
