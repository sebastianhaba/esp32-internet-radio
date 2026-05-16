#ifndef STATION_H
#define STATION_H

#include <Arduino.h>

struct Station {
    char name[32];
    char url[128];
};

#endif
