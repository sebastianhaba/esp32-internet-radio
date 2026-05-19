#include "DisplaySystem.h"
#include "ArchivoNarrowRegular5.h"
#include "ArchivoNarrowRegular8.h"

void DisplaySystem::begin() 
{
    _tft.init();
    _tft.setRotation(1);
    _spr = new (_sprBuf) TFT_eSprite(&_tft);
    _spr->loadFont((uint8_t*)ArchivoNarrowRegular8);
    _spr->createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    _dirty = true;
}

void DisplaySystem::clearScreen(uint16_t color) 
{
    _spr->fillSprite(color);
}

void DisplaySystem::setTextWrap(bool wrap) {
    _spr->setTextWrap(wrap);
}

void DisplaySystem::drawText(uint8_t x, uint8_t y, const String& text, uint16_t color) 
{
    _spr->setCursor(x, y);
    _spr->setTextColor(color);
    _spr->print(text);
}

int16_t DisplaySystem::textWidth(const String& text) 
{
    return _spr->textWidth(text);
}


void DisplaySystem::reloadFont(uint8_t size) 
{
    _spr->unloadFont();
    
    if(size == 5) {
        _spr->loadFont((uint8_t*)ArchivoNarrowRegular5);
    }
    else {
        _spr->loadFont((uint8_t*)ArchivoNarrowRegular8);
    }
}

void DisplaySystem::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
    _spr->drawRect(x, y, w, h, color);
}

void DisplaySystem::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
    _spr->fillRect(x, y, w, h, color);
}

void DisplaySystem::drawHLine(int16_t x, int16_t y, int16_t size, uint16_t color) 
{
    _spr->drawFastHLine(x, y, size, color);
}

void DisplaySystem::flush() 
{
    _spr->pushSprite(0, 0);
    _dirty = false;
}

void DisplaySystem::markDirty() 
{
    _dirty = true;
}

bool DisplaySystem::isDirty() const 
{
    return _dirty;
}
