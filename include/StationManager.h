#ifndef STATION_MANAGER_H
#define STATION_MANAGER_H

#include <Arduino.h>
#include "Station.h"

class StationManager {
public:
    StationManager(Station* stations, uint8_t count);

    void next();
    void prev();

    uint8_t currentIndex() const;
    const Station& current() const;
    const Station& at(uint8_t index) const;
    uint8_t count() const;
    void select(uint8_t index);

private:
    Station* _stations;
    uint8_t  _count;
    uint8_t  _currentIndex = 0;
};

#endif
