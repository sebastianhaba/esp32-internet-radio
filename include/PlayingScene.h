#ifndef PLAYING_SCENE_H
#define PLAYING_SCENE_H

#include "Scene.h"

class PlayingScene : public Scene {
public:
    void onEnter(SceneManager& manager) override;
    void draw(SceneManager& manager) override;
    void handleEvent(RotarySystem::Event ev, SceneManager& manager) override;

private:
    String   _lastTitle;
    int16_t  _scrollPixel = 0;
    uint32_t _lastFrameMs = 0;

    static constexpr int16_t MARGIN      = 10;
    static constexpr int16_t TITLE_Y     = 48;
    static constexpr int16_t SCROLL_STEP = 1;
    static constexpr uint16_t FRAME_MS   = 33;
    static constexpr int16_t SCROLL_GAP  = 30;
};

#endif
