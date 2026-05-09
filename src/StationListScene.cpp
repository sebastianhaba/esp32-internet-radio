#include "StationListScene.h"
#include "SceneManager.h"

static constexpr int16_t MARGIN = 10;

void StationListScene::onEnter(SceneManager& manager) {
    _selectionIndex = manager.stations().currentIndex();
    manager.display().markDirty();
}

void StationListScene::draw(SceneManager& manager) {
    auto& display = manager.display();
    auto& stations = manager.stations();

    String label = "< " + String(stations.at(_selectionIndex).name) + " >";

    display.clear();
    display.setTextColor(TFT_WHITE);

    int16_t labelX = (DisplaySystem::WIDTH - display.textWidth(label)) / 2;
    if (labelX < MARGIN) labelX = MARGIN;
    display.setCursor(labelX, 60);
    display.print(label);

    display.flush();
}

void StationListScene::handleEvent(RotarySystem::Event ev, SceneManager& manager) {
    if (ev == RotarySystem::EV_CW) {
        _selectionIndex = (_selectionIndex + 1) % manager.stations().count();
        manager.display().markDirty();
    } else if (ev == RotarySystem::EV_CCW) {
        if (_selectionIndex == 0) {
            _selectionIndex = manager.stations().count() - 1;
        } else {
            _selectionIndex--;
        }
        manager.display().markDirty();
    } else if (ev == RotarySystem::EV_PRESS) {
        manager.stations().select(_selectionIndex);
        manager.switchTo(SceneId::Loading);
    } else if (ev == RotarySystem::EV_LONG) {
        manager.switchTo(SceneId::Playing);
    }
}
