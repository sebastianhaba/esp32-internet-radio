#ifndef DISPLAY_SYSTEM_H
#define DISPLAY_SYSTEM_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <new>

class DisplaySystem {
public:
    void begin();

    void clearScreen(uint16_t color = TFT_BLACK);
    void setTextWrap(bool wrap);
    void drawText(int16_t x, int16_t y, const String& text, uint16_t color = TFT_WHITE);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawHLine(int16_t x, int16_t y, int16_t size, uint16_t color);
    
    int16_t textWidth(const String& text);
    void reloadFont(uint8_t size);

    void flush();

    void markDirty();
    bool isDirty() const;

    static constexpr int16_t SCREEN_WIDTH  = 160;
    static constexpr int16_t SCREEN_HEIGHT = 128;

private:
    TFT_eSPI _tft;
    alignas(TFT_eSprite) uint8_t _sprBuf[sizeof(TFT_eSprite)];
    TFT_eSprite* _spr = nullptr;
    bool _dirty = true;
};

#endif
