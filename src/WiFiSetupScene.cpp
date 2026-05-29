#include "WiFiSetupScene.h"
#include "SceneManager.h"

void WiFiSetupScene::setState(State s) { _state = s; }
void WiFiSetupScene::setSsid(const String& ssid) { _ssid = ssid; }
void WiFiSetupScene::setApSsid(const String& apSsid) { _apSsid = apSsid; }
void WiFiSetupScene::setApPassword(const String& apPassword) { _apPassword = apPassword; }
void WiFiSetupScene::setApIp(const String& apIp) { _apIp = apIp; }
void WiFiSetupScene::setConnectingProgress(uint8_t seconds) { _connectingSec = seconds; }

void WiFiSetupScene::onEnter(SceneManager& manager) {
    manager.display().markDirty();
}

void WiFiSetupScene::draw(SceneManager& manager) {
    auto& display = manager.display();
    display.clearScreen();
    display.reloadFont(5);

    if (_state == State::Connecting) {
        display.drawText(10, 20, "Czekaj...");
        display.drawText(10, 40, "Laczenie z:");
        display.drawText(10, 55, _ssid);
        display.drawText(10, 80, String(_connectingSec) + "s");
    } else {
        display.drawText(10, 15, "Konfiguracja WiFi");
        display.drawText(10, 40, _apIp);
        display.drawText(10, 60, "Polacz z AP:");
        display.drawText(10, 75, _apSsid);
        display.drawText(10, 95, "Haslo: " + _apPassword);
    }

    display.flush();
}

void WiFiSetupScene::handleEvent(RotarySystem::Event ev, SceneManager& manager) {
    (void)ev;
    (void)manager;
}
