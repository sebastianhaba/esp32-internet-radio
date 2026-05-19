#include "LoadingScene.h"
#include "SceneManager.h"

void LoadingScene::onEnter(SceneManager& manager) {
    _startMs = millis();
    manager.audio().play(manager.stations().current().url);
    manager.display().markDirty();
}

void LoadingScene::draw(SceneManager& manager) {
    constexpr uint32_t TIMEOUT_MS = 5000;
    
    auto& display = manager.display();

    if (manager.audio().state() == AudioSystem::State::Connected) {
        manager.switchTo(SceneId::Playing);
        return;
    }

    if (millis() - _startMs > TIMEOUT_MS) {
        manager.switchTo(SceneId::StationList);
        return;
    }

    display.clearScreen();
    display.drawText(30, 60, "Wczytywanie...");
    display.flush();
}

void LoadingScene::handleEvent(RotarySystem::Event ev, SceneManager& manager) {
    (void)ev;
    (void)manager;
}
