#ifndef STATION_LIST_SCENE_H
#define STATION_LIST_SCENE_H

#include "Scene.h"

class StationListScene : public Scene {
public:
    void onEnter(SceneManager& manager) override;
    void draw(SceneManager& manager) override;
    void handleEvent(RotarySystem::Event ev, SceneManager& manager) override;

private:
    uint8_t _selectionIndex = 0;
};

#endif
