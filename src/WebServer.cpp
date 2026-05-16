#include "WebServer.h"
#include "StationManager.h"
#include <LittleFS.h>

void RadioWebServer::begin(StationManager* stations) {
    _stations = stations;
    _server.begin();
    Serial.println("Web server started on port 80");
}

void RadioWebServer::handleClient() {
    WiFiClient client = _server.accept();
    if (!client) return;

    String req = client.readStringUntil('\r');
    client.readStringUntil('\n');

    String method, path;
    int sp1 = req.indexOf(' ');
    int sp2 = req.indexOf(' ', sp1 + 1);
    if (sp1 > 0 && sp2 > sp1) {
        method = req.substring(0, sp1);
        path   = req.substring(sp1 + 1, sp2);
    }

    // Read headers
    String contentType;
    int contentLength = 0;
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r" || line.length() <= 1) break;
        if (line.startsWith("Content-Length:")) {
            contentLength = line.substring(15).toInt();
        }
    }

    // Read body if any
    String body;
    if (contentLength > 0) {
        body = client.readString();
    }

    if (method == "GET" && path == "/") {
        handleRoot(client);
    } else if (method == "GET" && path == "/stations") {
        handleStations(client);
    } else if (method == "POST" && path == "/save") {
        handleSave(client, body);
    } else {
        send200(client, "text/plain", "404");
    }

    client.stop();
}

void RadioWebServer::handleRoot(WiFiClient& client) {
    File f = LittleFS.open("/index.html", "r");
    if (f && !f.isDirectory()) {
        String html = f.readString();
        f.close();
        send200(client, "text/html", html);
    } else {
        send200(client, "text/html", "<h1>ESP32 Radio</h1><p>/index.html not found</p>");
    }
}

void RadioWebServer::handleStations(WiFiClient& client) {
    File f = LittleFS.open("/stations.txt", "r");
    String content;
    if (f) {
        content = f.readString();
        f.close();
    }
    send200(client, "text/plain", content);
}

void RadioWebServer::handleSave(WiFiClient& client, const String& body) {
    String data = body;

    if (data.startsWith("stations=")) {
        data = data.substring(9);
    }

    // Full URL decode
    String decoded;
    decoded.reserve(data.length());
    for (unsigned i = 0; i < data.length(); i++) {
        if (data[i] == '%' && i + 2 < data.length()) {
            char c1 = data[i + 1], c2 = data[i + 2];
            char val = 0;
            if (c1 >= '0' && c1 <= '9') val = (c1 - '0') << 4;
            else if (c1 >= 'A' && c1 <= 'F') val = (c1 - 'A' + 10) << 4;
            else if (c1 >= 'a' && c1 <= 'f') val = (c1 - 'a' + 10) << 4;
            if (c2 >= '0' && c2 <= '9') val |= (c2 - '0');
            else if (c2 >= 'A' && c2 <= 'F') val |= (c2 - 'A' + 10);
            else if (c2 >= 'a' && c2 <= 'f') val |= (c2 - 'a' + 10);
            decoded += (char)val;
            i += 2;
        } else if (data[i] == '+') {
            decoded += ' ';
        } else {
            decoded += data[i];
        }
    }
    data = decoded;

    if (data.length() == 0) {
        send200(client, "text/plain", "Empty");
        return;
    }

    File f = LittleFS.open("/stations.txt", "w");
    if (!f) {
        send200(client, "text/plain", "Write error");
        return;
    }
    f.print(data);
    f.close();

    _stations->loadFromFile(LittleFS, "/stations.txt");

    send200(client, "text/plain", "OK");
    Serial.println("Stations saved and reloaded");
}

void RadioWebServer::send200(WiFiClient& client, const char* type, const String& body) {
    client.print("HTTP/1.1 200 OK\r\nContent-Type: ");
    client.print(type);
    client.print("\r\nConnection: close\r\n\r\n");
    client.print(body);
}
