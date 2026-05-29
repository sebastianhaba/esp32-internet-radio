#ifndef WIFI_SETUP_SCENE_H
#define WIFI_SETUP_SCENE_H

#include "Scene.h"

class WiFiSetupScene : public Scene {
public:
    enum class State { Connecting, ApActive };

    void setState(State s);
    void setSsid(const String& ssid);
    void setApSsid(const String& apSsid);
    void setApPassword(const String& apPassword);
    void setApIp(const String& apIp);
    void setConnectingProgress(uint8_t seconds);

    void onEnter(SceneManager& manager) override;
    void draw(SceneManager& manager) override;
    void handleEvent(RotarySystem::Event ev, SceneManager& manager) override;

private:
    State   _state = State::Connecting;
    String  _ssid;
    String  _apSsid;
    String  _apPassword;
    String  _apIp;
    uint8_t _connectingSec = 0;
};

#endif
