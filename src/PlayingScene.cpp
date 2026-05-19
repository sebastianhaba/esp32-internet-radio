#include "PlayingScene.h"
#include "SceneManager.h"
#include <WiFi.h>

static constexpr int16_t  VU_SEG         = 8;
static constexpr int16_t  VU_GAP         = 2;
static constexpr int16_t  VU_STEP        = VU_SEG + VU_GAP;
static constexpr int16_t  VU_NUM         = 15;
static constexpr int16_t  VU_H           = 8;
static constexpr int16_t  VU_MARGIN      = 6;
static constexpr int16_t  VU_TOP_Y       = 85;
static constexpr int16_t  VU_BOT_Y       = 95;
static constexpr int16_t  MARGIN         = 10;
static constexpr int16_t  TITLE_Y        = 40;
static constexpr int16_t  SCROLL_STEP    = 1;
static constexpr uint16_t FRAME_MS       = 33;
static constexpr int16_t  SCROLL_GAP     = 30;

static void drawLadder(DisplaySystem& d, int active, int x, int y) 
{
    for (int i = 0; i < VU_NUM; i++) 
    {
        if (i < active) 
        {
            uint16_t col = i < 10 ? TFT_GREEN : i < 12 ? TFT_YELLOW : TFT_RED;
            d.fillRect(x, y, VU_SEG, VU_H, col);
        } 
        
        else 
        {
            d.drawRect(x, y, VU_SEG, VU_H, 0x4208);
        }
        x += VU_STEP;
    }
}

void PlayingScene::onEnter(SceneManager& manager) 
{
    manager.display().markDirty();
}

void PlayingScene::draw(SceneManager& manager) 
{
    auto& display = manager.display();
    auto& audio   = manager.audio();
    String title  = audio.getStreamTitle();

    if (title != _lastTitle) 
    {
        _lastTitle = title;
        _scrollPixel = 0;
    }

    int16_t availW  = DisplaySystem::SCREEN_WIDTH - 2 * MARGIN;
    bool needsScroll = (title.length() > 0);
    bool needsContinuous = needsScroll || (audio.state() == AudioSystem::State::Connected);

    if (!needsContinuous) 
    {
        if (!display.isDirty() && title.length() > 0) {
            return;
        }
    } 
    else 
    {
        uint32_t now = millis();
        if (now - _lastFrameMs < FRAME_MS)  {
            return;
        }

        _lastFrameMs = now;
    }

    // --- Background ---
    display.clearScreen();

    // --- TOP BAR (GLCD) ---
    display.reloadFont(5);

    String timeStr = manager.timeString();
    String volStr  = String(audio.getVolumePercent()) + "%";
    String nameStr = String(manager.stations().current().name);

    int16_t timeW = display.textWidth(timeStr);
    int16_t volW  = display.textWidth(volStr);
    int16_t nameW = display.textWidth(nameStr);

    display.drawText(2, 2, timeStr);
    display.drawText(DisplaySystem::SCREEN_WIDTH - volW - 2, 2, volStr);

    int16_t nameX = (DisplaySystem::SCREEN_WIDTH - nameW) / 2;
    if (nameX < timeW + 4) 
    {
        nameX = timeW + 4;
    }
    
    if (nameX + nameW > DisplaySystem::SCREEN_WIDTH - volW - 4) 
    { 
        nameX = DisplaySystem::SCREEN_WIDTH - volW - 4 - nameW;
    }

    display.drawText(nameX, 2, nameStr);
    display.drawHLine(0, 12, DisplaySystem::SCREEN_WIDTH, TFT_WHITE);

    // --- VU BARS ---
    int16_t vuX = VU_MARGIN;
    uint16_t vuRaw = audio.getVUlevel();
    int vuL = (vuRaw & 0xFF) * VU_NUM / 256;
    int vuR = ((vuRaw >> 8) & 0xFF) * VU_NUM / 256;
    drawLadder(display, vuL, vuX, VU_TOP_Y);
    drawLadder(display, vuR, vuX, VU_BOT_Y);

    // --- INFO BAR ---
    String info = WiFi.localIP().toString() + " | " + audio.getCodecname() + " | " + (audio.getBitRate() / 1000) + " kbps";
    int16_t infoW = display.textWidth(info);

    display.drawHLine(0, 111, DisplaySystem::SCREEN_WIDTH, TFT_WHITE);
    display.drawText((DisplaySystem::SCREEN_WIDTH - infoW) / 2, 115, info);

    // --- STREAM TITLE (smooth font) ---
    display.reloadFont(8);
    int16_t textW = display.textWidth(title);
    bool needsScrollLocal = (title.length() > 0 && textW > availW);

    if (title.length() > 0) 
    {
        if (!needsScrollLocal) 
        {
            int16_t titleX = (DisplaySystem::SCREEN_WIDTH - textW) / 2;
            if (titleX < MARGIN) 
            {
                titleX = MARGIN;
            }
            display.drawText(titleX, TITLE_Y, title);
        } 
        else 
        {
            int32_t wrapRange = textW + SCROLL_GAP;
            _scrollPixel += SCROLL_STEP;
            if (_scrollPixel >= wrapRange) 
            { 
                _scrollPixel -= wrapRange;
            }
            
            int16_t x1 = MARGIN - _scrollPixel;
            int16_t x2 = x1 + textW + SCROLL_GAP;

            display.setTextWrap(false);
            display.drawText(x1, TITLE_Y, title);

            if (x2 < DisplaySystem::SCREEN_WIDTH - MARGIN) 
            {
                display.drawText(x2, TITLE_Y, title);
            }
            display.setTextWrap(true);
        }
    }

    display.flush();
}

void PlayingScene::handleEvent(RotarySystem::Event ev, SceneManager& manager) 
{
    if (ev == RotarySystem::RIGHT) 
    {
        manager.audio().volumeUp();
        manager.display().markDirty();
    } 
    else if (ev == RotarySystem::LEFT) 
    {
        manager.audio().volumeDown();
        manager.display().markDirty();
    } 
    else if (ev == RotarySystem::PRESS) 
    {
        manager.switchTo(SceneId::StationList);
    }
}
