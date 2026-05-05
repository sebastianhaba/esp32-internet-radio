#include "PlayingScene.h"
#include "SceneManager.h"

void PlayingScene::onEnter(SceneManager& manager) {
    manager.display().markDirty();
}

void PlayingScene::draw(SceneManager& manager) {
    auto& display = manager.display();
    auto& audio   = manager.audio();

    if (!display.isDirty() && audio.getStreamTitle().length() > 0) {
        return;
    }

    display.clear();
    display.setTextColor(ST77XX_WHITE);

    display.setCursor(2, 2);
    display.print(manager.timeString());

    int volPercent = audio.getVolumePercent();
    display.drawRect(54, 2, 102, 12, ST77XX_WHITE);
    display.fillRect(55, 3, volPercent, 10, ST77XX_GREEN);

    int16_t titleX = (DisplaySystem::WIDTH - (int)DisplaySystem::toAscii(audio.getStreamTitle()).length() * 6) / 2;
    if (titleX < 0) titleX = 2;
    display.setCursor(titleX, 48);
    display.print(DisplaySystem::toAscii(audio.getStreamTitle()));

    display.flush();
}

void PlayingScene::handleEvent(RotarySystem::Event ev, SceneManager& manager) {
    if (ev == RotarySystem::EV_CW) {
        manager.audio().volumeUp();
        manager.display().markDirty();
    } else if (ev == RotarySystem::EV_CCW) {
        manager.audio().volumeDown();
        manager.display().markDirty();
    } else if (ev == RotarySystem::EV_LONG) {
        manager.switchTo(SceneId::StationList);
    }
}
