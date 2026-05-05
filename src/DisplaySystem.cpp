#include "DisplaySystem.h"

void DisplaySystem::begin() {
    SPI.begin(PIN_TFT_SCK, -1, PIN_TFT_MOSI, PIN_TFT_CS);
    _tft.initR(INITR_BLACKTAB);
    _tft.setRotation(1);
    _dirty = true;
}

void DisplaySystem::clear() {
    _canvas.fillScreen(0x0000);
}

void DisplaySystem::fillScreen(uint16_t color) {
    _canvas.fillScreen(color);
}

void DisplaySystem::setCursor(int16_t x, int16_t y) {
    _canvas.setCursor(x, y);
}

void DisplaySystem::setTextColor(uint16_t color) {
    _canvas.setTextColor(color);
}

void DisplaySystem::print(const String& text) {
    _canvas.print(text);
}

void DisplaySystem::print(int num) {
    _canvas.print(num);
}

void DisplaySystem::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _canvas.drawRect(x, y, w, h, color);
}

void DisplaySystem::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _canvas.fillRect(x, y, w, h, color);
}

int16_t DisplaySystem::getCursorX() const {
    return _canvas.getCursorX();
}

int16_t DisplaySystem::getCursorY() const {
    return _canvas.getCursorY();
}

void DisplaySystem::flush() {
    _tft.drawRGBBitmap(0, 0, _canvas.getBuffer(), WIDTH, HEIGHT);
    _dirty = false;
}

void DisplaySystem::markDirty() {
    _dirty = true;
}

bool DisplaySystem::isDirty() const {
    return _dirty;
}

String DisplaySystem::toAscii(const String& text) {
    String from = "ąćęłńóśźżĄĆĘŁŃÓŚŹŻ";
    String to   = "acelnoszzACELNOSZZ";
    String result = text;
    for (int i = 0; i < from.length(); i++) {
        result.replace(from.substring(i, i+1), to.substring(i, i+1));
    }
    return result;
}
