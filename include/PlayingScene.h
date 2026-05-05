#ifndef PLAYING_SCENE_H
#define PLAYING_SCENE_H

#include "Scene.h"

class PlayingScene : public Scene {
public:
    void onEnter(SceneManager& manager) override;
    void draw(SceneManager& manager) override;
    void handleEvent(RotarySystem::Event ev, SceneManager& manager) override;
};

#endif
