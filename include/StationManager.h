#ifndef STATION_MANAGER_H
#define STATION_MANAGER_H

#include <Arduino.h>
#include <FS.h>
#include "Station.h"

class StationManager {
public:
    StationManager();

    bool loadFromFile(fs::FS &fs, const char* path);
    void add(const char* name, const char* url);

    void next();
    void prev();

    uint8_t currentIndex() const;
    const Station& current() const;
    const Station& at(uint8_t index) const;
    uint8_t count() const;
    void select(uint8_t index);

private:
    static constexpr uint8_t MAX_STATIONS = 20;
    Station _stations[MAX_STATIONS];
    uint8_t  _count = 0;
    uint8_t  _currentIndex = 0;
};

#endif
