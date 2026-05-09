#include "DisplaySystem.h"
#include "DejaVuSansMono12.h"

void DisplaySystem::begin() {
    Serial.println("  [lcd] init()...");
    Serial.flush();
    _tft.init();
    Serial.println("  [lcd] setRotation...");
    Serial.flush();
    _tft.setRotation(1);
    Serial.println("  [lcd] loadFont...");
    Serial.flush();
    _tft.loadFont(DejaVuSansMono12);
    Serial.println("  [lcd] fillScreen TFT_BLUE...");
    Serial.flush();
    _tft.fillScreen(TFT_BLUE);
    delay(1000);
    Serial.println("  [lcd] fillScreen TFT_RED...");
    Serial.flush();
    _tft.fillScreen(TFT_RED);
    delay(1000);
    Serial.println("  [lcd] fillScreen TFT_BLACK...");
    Serial.flush();
    _tft.fillScreen(TFT_BLACK);
    Serial.println("  [lcd] OK");
    Serial.flush();
    _dirty = true;
}

void DisplaySystem::clear() {
    _tft.fillScreen(TFT_BLACK);
}

void DisplaySystem::fillScreen(uint16_t color) {
    _tft.fillScreen(color);
}

void DisplaySystem::setCursor(int16_t x, int16_t y) {
    _tft.setCursor(x, y);
}

void DisplaySystem::setTextColor(uint16_t color) {
    _tft.setTextColor(color);
}

void DisplaySystem::setTextWrap(bool wrap) {
    _tft.setTextWrap(wrap);
}

void DisplaySystem::print(const String& text) {
    _tft.print(text);
}

void DisplaySystem::print(int num) {
    _tft.print(num);
}

int16_t DisplaySystem::textWidth(const String& text) {
    return _tft.textWidth(text);
}

void DisplaySystem::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _tft.drawRect(x, y, w, h, color);
}

void DisplaySystem::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _tft.fillRect(x, y, w, h, color);
}

int16_t DisplaySystem::getCursorX() {
    return _tft.getCursorX();
}

int16_t DisplaySystem::getCursorY() {
    return _tft.getCursorY();
}

void DisplaySystem::flush() {
    _dirty = false;
}

void DisplaySystem::markDirty() {
    _dirty = true;
}

bool DisplaySystem::isDirty() const {
    return _dirty;
}
