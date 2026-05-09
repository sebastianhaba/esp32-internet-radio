#ifndef DISPLAY_SYSTEM_H
#define DISPLAY_SYSTEM_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <new>

class DisplaySystem {
public:
    void begin();

    void clear();
    void fillScreen(uint16_t color);
    void setCursor(int16_t x, int16_t y);
    void setTextColor(uint16_t color);
    void setTextWrap(bool wrap);
    void print(const String& text);
    void print(int num);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    int16_t getCursorX();
    int16_t getCursorY();
    int16_t textWidth(const String& text);

    void flush();

    void markDirty();
    bool isDirty() const;

    static constexpr int16_t WIDTH  = 160;
    static constexpr int16_t HEIGHT = 128;

private:
    TFT_eSPI _tft;
    alignas(TFT_eSprite) uint8_t _sprBuf[sizeof(TFT_eSprite)];
    TFT_eSprite* _spr = nullptr;
    bool _dirty = true;
};

#endif
