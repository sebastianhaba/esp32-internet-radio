#include "StationManager.h"

StationManager::StationManager(Station* stations, uint8_t count)
    : _stations(stations), _count(count) {}

void StationManager::next() {
    _currentIndex = (_currentIndex + 1) % _count;
}

void StationManager::prev() {
    _currentIndex = (_currentIndex == 0) ? _count - 1 : _currentIndex - 1;
}

uint8_t StationManager::currentIndex() const {
    return _currentIndex;
}

const Station& StationManager::current() const {
    return _stations[_currentIndex];
}

const Station& StationManager::at(uint8_t index) const {
    return _stations[index];
}

uint8_t StationManager::count() const {
    return _count;
}

void StationManager::select(uint8_t index) {
    if (index < _count) {
        _currentIndex = index;
    }
}
