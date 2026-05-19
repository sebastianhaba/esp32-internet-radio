#include "StationManager.h"

StationManager::StationManager() {}

bool StationManager::loadFromFile(fs::FS &fs, const char* path) 
{
    File file = fs.open(path, FILE_READ);
    if (!file || file.isDirectory()) { 
        return false;
    }

    _count = 0;
    while (file.available() && _count < MAX_STATIONS) 
    {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() == 0 || line.startsWith("#")) { 
            continue;
        }

        int sep = line.indexOf('|');
        if (sep <= 0 || sep >= line.length() - 1) 
        { 
            continue; 
        }

        String name = line.substring(0, sep);
        String url  = line.substring(sep + 1);
        name.trim();
        url.trim();

        strncpy(_stations[_count].name, name.c_str(), sizeof(Station::name) - 1);
        _stations[_count].name[sizeof(Station::name) - 1] = '\0';
        strncpy(_stations[_count].url, url.c_str(), sizeof(Station::url) - 1);
        _stations[_count].url[sizeof(Station::url) - 1] = '\0';
        _count++;
    }

    file.close();
    return _count > 0;
}

void StationManager::add(const char* name, const char* url) 
{
    if (_count >= MAX_STATIONS) 
    {
         return;
    }
    
    strncpy(_stations[_count].name, name, sizeof(Station::name) - 1);
    _stations[_count].name[sizeof(Station::name) - 1] = '\0';
    strncpy(_stations[_count].url, url, sizeof(Station::url) - 1);
    _stations[_count].url[sizeof(Station::url) - 1] = '\0';
    _count++;
}

void StationManager::next() 
{
    if (_count == 0) 
    {
         return;
    }
    _currentIndex = (_currentIndex + 1) % _count;
}

void StationManager::prev() 
{
    if (_count == 0) 
    { 
        return; 
    }
    
    _currentIndex = (_currentIndex == 0) ? _count - 1 : _currentIndex - 1;
}

uint8_t StationManager::currentIndex() const 
{
    return _currentIndex;
}

const Station& StationManager::current() const 
{
    return _stations[_currentIndex];
}

const Station& StationManager::at(uint8_t index) const 
{
    return _stations[index];
}

uint8_t StationManager::count() const 
{
    return _count;
}

void StationManager::select(uint8_t index) 
{
    if (index < _count) 
    {
        _currentIndex = index;
    }
}
