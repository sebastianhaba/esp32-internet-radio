#include "SceneManager.h"

SceneManager::SceneManager(AudioSystem& audio, StationManager& stations, DisplaySystem& display)
    : _audio(audio), _stations(stations), _display(display) {}

void SceneManager::begin() {
    switchTo(SceneId::Playing);
}

void SceneManager::draw() {
    currentScene().draw(*this);
}

void SceneManager::handleEvent(RotarySystem::Event ev) {
    currentScene().handleEvent(ev, *this);
}

void SceneManager::switchTo(SceneId id) {
    currentScene().onExit();
    _currentId = id;
    currentScene().onEnter(*this);
}

void SceneManager::setTimeString(const String& time) {
    _timeStr = time;
}

const String& SceneManager::timeString() const {
    return _timeStr;
}

Scene& SceneManager::currentScene() {
    switch (_currentId) {
        case SceneId::Playing:     return _playingScene;
        case SceneId::StationList: return _stationListScene;
        case SceneId::Loading:     return _loadingScene;
    }
    return _playingScene;
}
