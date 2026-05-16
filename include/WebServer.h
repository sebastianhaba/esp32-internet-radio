#ifndef RADIO_WEB_SERVER_H
#define RADIO_WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>

class StationManager;

class RadioWebServer {
public:
    void begin(StationManager* stations);
    void handleClient();

private:
    WiFiServer _server{80};
    StationManager* _stations = nullptr;

    void handleRoot(WiFiClient& client);
    void handleStations(WiFiClient& client);
    void handleSave(WiFiClient& client, const String& body);
    void send200(WiFiClient& client, const char* type, const String& body);
};

#endif
