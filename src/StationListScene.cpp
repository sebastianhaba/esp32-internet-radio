#include "StationListScene.h"
#include "SceneManager.h"

void StationListScene::onEnter(SceneManager& manager) 
{
    _selectionIndex = manager.stations().currentIndex();
    manager.display().markDirty();
}

void StationListScene::draw(SceneManager& manager) 
{
    constexpr int16_t MARGIN = 10;
    constexpr int16_t LABEL_Y = 60;
    
    auto& display = manager.display();
    auto& stations = manager.stations();

    String label = "< " + String(stations.at(_selectionIndex).name) + " >";
    int16_t labelX = (DisplaySystem::SCREEN_WIDTH - display.textWidth(label)) / 2;

    if(labelX < MARGIN) 
    {
        labelX = MARGIN;
    }

    display.clearScreen();
    display.drawText(labelX, LABEL_Y, label);
    display.flush();
}

void StationListScene::handleEvent(RotarySystem::Event ev, SceneManager& manager) {
    if(ev == RotarySystem::RIGHT) 
    {
        _selectionIndex = (_selectionIndex + 1) % manager.stations().count();
        manager.display().markDirty();
    } 
    else if(ev == RotarySystem::LEFT) 
    {
        if (_selectionIndex == 0) 
        {
            _selectionIndex = manager.stations().count() - 1;
        } 
        else 
        {
            _selectionIndex--;
        }
        manager.display().markDirty();
    } 
    else if(ev == RotarySystem::CLICK)
    {
        manager.stations().select(_selectionIndex);
        manager.switchTo(SceneId::Loading);
    } 
    else if(ev == RotarySystem::PRESS) 
    {
        manager.switchTo(SceneId::Playing);
    }
}
