#ifndef DISPLAY_SYSTEM_H
#define DISPLAY_SYSTEM_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

class DisplaySystem {
public:
    void begin();

    void clear();
    void fillScreen(uint16_t color);
    void setCursor(int16_t x, int16_t y);
    void setTextColor(uint16_t color);
    void print(const String& text);
    void print(int num);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    int16_t getCursorX() const;
    int16_t getCursorY() const;

    void flush();

    void markDirty();
    bool isDirty() const;

    static String toAscii(const String& text);

    static constexpr int16_t WIDTH  = 160;
    static constexpr int16_t HEIGHT = 128;

private:
    static constexpr uint8_t PIN_TFT_SCK  = D0;
    static constexpr uint8_t PIN_TFT_MOSI = D1;
    static constexpr uint8_t PIN_TFT_DC   = D2;
    static constexpr uint8_t PIN_TFT_RST  = D3;
    static constexpr uint8_t PIN_TFT_CS   = D4;

    Adafruit_ST7735 _tft{PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST};
    GFXcanvas16 _canvas{WIDTH, HEIGHT};
    bool _dirty = true;
};

#endif
