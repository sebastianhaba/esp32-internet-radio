#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <Arduino.h>
#include "RotarySystem.h"
#include "AudioSystem.h"
#include "StationManager.h"
#include "DisplaySystem.h"
#include "Scene.h"
#include "PlayingScene.h"
#include "StationListScene.h"
#include "LoadingScene.h"
#include "WiFiSetupScene.h"

enum class SceneId { Playing, StationList, Loading, WiFiSetup };

class SceneManager {
public:
    SceneManager(AudioSystem& audio, StationManager& stations, DisplaySystem& display);

    void begin();
    void draw();
    void handleEvent(RotarySystem::Event ev);
    void switchTo(SceneId id);

    void setTimeString(const String& time);
    const String& timeString() const;

    AudioSystem& audio() { return _audio; }
    StationManager& stations() { return _stations; }
    DisplaySystem& display() { return _display; }
    WiFiSetupScene& wifiSetupScene();

private:
    Scene& currentScene();

    AudioSystem&    _audio;
    StationManager& _stations;
    DisplaySystem&  _display;

    SceneId _currentId = SceneId::Playing;
    PlayingScene     _playingScene;
    StationListScene _stationListScene;
    LoadingScene     _loadingScene;
    WiFiSetupScene   _wifiSetupScene;

    String _timeStr;
};

#endif
