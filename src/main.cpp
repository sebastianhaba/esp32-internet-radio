#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <LittleFS.h>

#include "Station.h"
#include "StationManager.h"
#include "AudioSystem.h"
#include "RotarySystem.h"
#include "DisplaySystem.h"
#include "SceneManager.h"
#include "WebServer.h"

#define PIN_I2S_LRC  D0
#define PIN_I2S_BCLK D9
#define PIN_I2S_DOUT D1

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

AudioSystem     audioSystem;
RotarySystem    rotarySystem;
DisplaySystem   displaySystem;
StationManager  stationManager;
SceneManager    sceneManager(audioSystem, stationManager, displaySystem);
RadioWebServer  radioWeb;

int lastMinute = -1;
bool apMode = false;

static String getLocalTimeStr() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return "00:00";
    char buf[6];
    strftime(buf, sizeof(buf), "%H:%M", &timeinfo);
    return String(buf);
}

static bool readWifiConfig(String& ssid, String& password) {
    File f = LittleFS.open("/wifi.txt", "r");
    if (!f || f.isDirectory()) return false;

    String content = f.readString();
    f.close();

    int ssidStart = content.indexOf("\"ssid\":\"");
    if (ssidStart < 0) return false;
    ssidStart += 8;
    int ssidEnd = content.indexOf("\"", ssidStart);
    if (ssidEnd < 0) return false;
    ssid = content.substring(ssidStart, ssidEnd);

    int passStart = content.indexOf("\"password\":\"");
    if (passStart < 0) return false;
    passStart += 12;
    int passEnd = content.indexOf("\"", passStart);
    if (passEnd < 0) return false;
    password = content.substring(passStart, passEnd);

    return (ssid.length() > 0);
}

static String apSsidFromMac() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char buf[24];
    snprintf(buf, sizeof(buf), "ESP32-Radio-%02X%02X", mac[4], mac[5]);
    return String(buf);
}

static void enterApMode() {
    String apSsid = apSsidFromMac();
    String apPassword = "radio123";
    IPAddress apIP(192, 168, 100, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(apIP, apIP, subnet);
    WiFi.softAP(apSsid.c_str(), apPassword.c_str(), 1, 0, 1, false);

    Serial.println("\nAP mode started");
    Serial.printf("SSID: %s\n", apSsid.c_str());
    Serial.printf("Password: %s\n", apPassword.c_str());
    Serial.printf("IP: 192.168.100.1\n");

    auto& wfScene = sceneManager.wifiSetupScene();
    wfScene.setState(WiFiSetupScene::State::ApActive);
    wfScene.setApSsid(apSsid);
    wfScene.setApPassword(apPassword);
    wfScene.setApIp("192.168.100.1");
    sceneManager.switchTo(SceneId::WiFiSetup);

    apMode = true;
    radioWeb.begin(&stationManager, true);
}

void setup() {
    pinMode(1, OUTPUT); digitalWrite(1, LOW); 
    pinMode(2, OUTPUT); digitalWrite(2, LOW);
    
    Serial.begin(115200);
    uint32_t serialStart = millis();
    while (!Serial && (millis() - serialStart < 3000)) { delay(10); }
    delay(100);

    Serial.println("\n===== START =====");
    Serial.printf("Total PSRAM: %u\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %u\n", ESP.getFreePsram());

    Serial.println("Initialize filesystem");
    if (!LittleFS.begin(true, "/littlefs", 10, "littlefs")) {
        Serial.println("LittleFS mount FAILED");
    } else {
        Serial.println("LittleFS mounted");
    }

    Serial.println("Load stations");
    if (!stationManager.loadFromFile(LittleFS, "/stations.txt")) {
        Serial.println("No stations file, using defaults");
        stationManager.add("Radio Nowy Swiat", "http://stream.nowyswiat.online/mp3");
    }
    Serial.printf("Loaded %u station(s)\n", stationManager.count());

    Serial.println("Initialize lcd");
    displaySystem.begin();

    String wifiSsid, wifiPass;
    bool hasWifiConfig = readWifiConfig(wifiSsid, wifiPass);

    if (hasWifiConfig) {
        Serial.println("Connecting to wifi");
        Serial.printf("SSID: %s\n", wifiSsid.c_str());

        auto& wfScene = sceneManager.wifiSetupScene();
        wfScene.setState(WiFiSetupScene::State::Connecting);
        wfScene.setSsid(wifiSsid);
        sceneManager.switchTo(SceneId::WiFiSetup);

        WiFi.disconnect();
        WiFi.mode(WIFI_MODE_STA);
        WiFi.begin(wifiSsid.c_str(), wifiPass.c_str());

        uint32_t connStart = millis();
        while (WiFi.status() != WL_CONNECTED && (millis() - connStart < 30000)) {
            delay(1000);
            Serial.print(".");
            int sec = (millis() - connStart) / 1000;
            wfScene.setConnectingProgress(sec);
            sceneManager.draw();
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWiFi connected");
            Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

            Serial.println("Initialize rotary encoder");
            rotarySystem.begin(D6, D7, D5);

            Serial.println("Initialize audio");
            audioSystem.begin(PIN_I2S_BCLK, PIN_I2S_LRC, PIN_I2S_DOUT);

            radioWeb.begin(&stationManager, false);

            Serial.println("Initialize time from network");
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

            Serial.println("Play first station");
            audioSystem.play(stationManager.current().url);

            sceneManager.switchTo(SceneId::Playing);
        } else {
            Serial.println("\nWiFi connection timeout");
            enterApMode();
        }
    } else {
        Serial.println("No wifi config found");
        enterApMode();
    }
}

void loop() {
    if (apMode) {
        radioWeb.handleClient();
        sceneManager.draw();
        return;
    }

    audioSystem.loop();
    radioWeb.handleClient();

    if (audioSystem.titleChanged()) {
        sceneManager.display().markDirty();
    }

    RotarySystem::Event ev;
    while (rotarySystem.popEvent(ev)) {
        sceneManager.handleEvent(ev);
    }

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        if (timeinfo.tm_min != lastMinute) {
            lastMinute = timeinfo.tm_min;
            sceneManager.setTimeString(getLocalTimeStr());
            sceneManager.display().markDirty();
        }
    }

    sceneManager.draw();
}
