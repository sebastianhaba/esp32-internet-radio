#include "DisplaySystem.h"
#include "DejaVuSansMono6.h"

void DisplaySystem::begin() {
    _tft.init();
    _tft.setRotation(1);
    _spr = new (_sprBuf) TFT_eSprite(&_tft);
    _spr->loadFont((uint8_t*)DejaVuSansMono6);
    _spr->createSprite(WIDTH, HEIGHT);
    _dirty = true;
}

void DisplaySystem::clear() {
    _spr->fillSprite(TFT_BLACK);
}

void DisplaySystem::fillScreen(uint16_t color) {
    _spr->fillSprite(color);
}

void DisplaySystem::setCursor(int16_t x, int16_t y) {
    _spr->setCursor(x, y);
}

void DisplaySystem::setTextColor(uint16_t color) {
    _spr->setTextColor(color);
}

void DisplaySystem::setTextWrap(bool wrap) {
    _spr->setTextWrap(wrap);
}

void DisplaySystem::print(const String& text) {
    _spr->print(text);
}

void DisplaySystem::print(int num) {
    _spr->print(num);
}

int16_t DisplaySystem::textWidth(const String& text) {
    return _spr->textWidth(text);
}

void DisplaySystem::setTextFont(uint8_t f) {
    _spr->setTextFont(f);
}

void DisplaySystem::unloadFont() {
    _spr->unloadFont();
}

void DisplaySystem::reloadFont() {
    _spr->loadFont((uint8_t*)DejaVuSansMono6);
}

void DisplaySystem::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _spr->drawRect(x, y, w, h, color);
}

void DisplaySystem::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _spr->fillRect(x, y, w, h, color);
}

int16_t DisplaySystem::getCursorX() {
    return _spr->getCursorX();
}

int16_t DisplaySystem::getCursorY() {
    return _spr->getCursorY();
}

void DisplaySystem::flush() {
    _spr->pushSprite(0, 0);
    _dirty = false;
}

void DisplaySystem::markDirty() {
    _dirty = true;
}

bool DisplaySystem::isDirty() const {
    return _dirty;
}
