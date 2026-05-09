#include "PlayingScene.h"
#include "SceneManager.h"

void PlayingScene::onEnter(SceneManager& manager) {
    manager.display().markDirty();
}

void PlayingScene::draw(SceneManager& manager) {
    auto& display = manager.display();
    auto& audio   = manager.audio();
    String title  = audio.getStreamTitle();

    if (title != _lastTitle) {
        _lastTitle = title;
        _scrollPixel = 0;
    }

    int16_t textW   = display.textWidth(title);
    int16_t availW  = DisplaySystem::WIDTH - 2 * MARGIN;
    bool needsScroll = (title.length() > 0 && textW > availW);

    if (!needsScroll) {
        if (!display.isDirty() && title.length() > 0) return;
    } else {
        uint32_t now = millis();
        if (now - _lastFrameMs < FRAME_MS) return;
        _lastFrameMs = now;
    }

    display.clear();
    display.setTextColor(TFT_WHITE);

    display.setCursor(2, 2);
    display.print(manager.timeString());

    int volPercent = audio.getVolumePercent();
    display.drawRect(54, 2, 102, 12, TFT_WHITE);
    display.fillRect(55, 3, volPercent, 10, TFT_GREEN);

    if (title.length() > 0) {
        if (!needsScroll) {
            int16_t titleX = (DisplaySystem::WIDTH - textW) / 2;
            if (titleX < MARGIN) titleX = MARGIN;
            display.setCursor(titleX, TITLE_Y);
        } else {
            int32_t scrollRange = textW + SCROLL_GAP;
            _scrollPixel += SCROLL_STEP;
            if (_scrollPixel >= scrollRange) _scrollPixel -= scrollRange;

            int16_t offset = (_scrollPixel > textW - availW) ? (textW - availW) : _scrollPixel;
            display.setTextWrap(false);
            display.setCursor(MARGIN - offset, TITLE_Y);
        }
        display.print(title);
        display.setTextWrap(true);
    }

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
