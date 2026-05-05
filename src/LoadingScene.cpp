#include "LoadingScene.h"
#include "SceneManager.h"

void LoadingScene::onEnter(SceneManager& manager) {
    _startMs = millis();
    manager.audio().play(manager.stations().current().url);
    manager.display().markDirty();
}

void LoadingScene::draw(SceneManager& manager) {
    auto& display = manager.display();

    if (manager.audio().state() == AudioSystem::State::Connected) {
        manager.switchTo(SceneId::Playing);
        return;
    }

    if (millis() - _startMs > TIMEOUT_MS) {
        manager.switchTo(SceneId::StationList);
        return;
    }

    display.clear();
    display.setTextColor(ST77XX_WHITE);
    display.setCursor(30, 60);
    display.print("Wczytywanie...");
    display.flush();
}

void LoadingScene::handleEvent(RotarySystem::Event ev, SceneManager& manager) {
    (void)ev;
    (void)manager;
}
