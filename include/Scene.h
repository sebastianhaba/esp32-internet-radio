#ifndef SCENE_H
#define SCENE_H

#include <Arduino.h>
#include "RotarySystem.h"

class SceneManager;

class Scene {
public:
    virtual ~Scene() = default;
    virtual void onEnter(SceneManager& manager) {}
    virtual void onExit() {}
    virtual void draw(SceneManager& manager) = 0;
    virtual void handleEvent(RotarySystem::Event ev, SceneManager& manager) = 0;
};

#endif
