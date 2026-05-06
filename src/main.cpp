#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

#include "Station.h"
#include "StationManager.h"
#include "AudioSystem.h"
#include "RotarySystem.h"
#include "DisplaySystem.h"
#include "SceneManager.h"

Station STATIONS[] = {
    {"Radio Nowy Swiat",        "http://stream.nowyswiat.online/mp3"},
    {"Radio 357",               "https://stream.radio357.pl/"},
    {"Polskie Radio Trójka",    "http://stream3.polskieradio.pl:8902/;stream.mp3"},
    {"RP Main Mix",             "http://stream.radioparadise.com/aac-128"},
    {"RP Mellow Mix",           "http://stream.radioparadise.com/mellow-128"},
    {"RP Rock Mix",             "http://stream.radioparadise.com/rock-128"},

};

#define PIN_I2S_LRC  D8
#define PIN_I2S_BCLK D9
#define PIN_I2S_DOUT D10

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

AudioSystem     audioSystem;
RotarySystem    rotarySystem;
DisplaySystem   displaySystem;
StationManager  stationManager(STATIONS, sizeof(STATIONS) / sizeof(STATIONS[0]));
SceneManager    sceneManager(audioSystem, stationManager, displaySystem);

int lastMinute = -1;

String getLocalTimeStr() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) return "00:00";
    char buf[6];
    strftime(buf, sizeof(buf), "%H:%M", &timeinfo);
    return String(buf);
}

void setup() {
    Serial.begin(115200);
    delay(2000);

    Serial.println("\n===== START =====");
    Serial.printf("Total PSRAM: %u\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %u\n", ESP.getFreePsram());

    Serial.println("Initialize lcd");
    displaySystem.begin();

    Serial.println("Initialize rotary encoder");
    rotarySystem.begin(D6, D7, D5);

    Serial.println("Initialize audio");
    audioSystem.begin(PIN_I2S_BCLK, PIN_I2S_LRC, PIN_I2S_DOUT);

    Serial.println("Connecting to wifi");
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());

    Serial.println("Initialize time from network");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    Serial.println("Play first station");
    audioSystem.play(stationManager.current().url);

    sceneManager.begin();
}

void loop() {
    audioSystem.loop();

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
