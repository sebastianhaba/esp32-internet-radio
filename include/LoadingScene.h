#ifndef LOADING_SCENE_H
#define LOADING_SCENE_H

#include "Scene.h"

class LoadingScene : public Scene {
public:
    void onEnter(SceneManager& manager) override;
    void draw(SceneManager& manager) override;
    void handleEvent(RotarySystem::Event ev, SceneManager& manager) override;

private:
    uint32_t _startMs = 0;
    static constexpr uint32_t TIMEOUT_MS = 5000;
};

#endif
