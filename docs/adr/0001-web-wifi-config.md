# Konfiguracja WiFi przez portal webowy zamiast secrets.ini

Zrezygnowaliśmy z podawania SSID i hasła przez `secrets.ini` (compile-time defines). Zamiast tego radio sprawdza przy starcie plik `/wifi.txt` na LittleFS. Jeśli plik nie istnieje lub połączenie nie powiedzie się w ciągu 30 sekund, radio przechodzi w tryb Access Point i udostępnia stronę konfiguracyjną pod `192.168.100.1`.

**Status**: accepted

**Rozważane alternatywy**: WiFiManager (biblioteka), captive portal, secrets.ini jako fallback. Wybrano własną implementację — minimalną, bez dodatkowych zależności, z własnym serwerem HTTP który już istniał w projekcie.

**Konsekwencje**: Inicjalizacja LCD musi nastąpić przed WiFi (żeby pokazać status). Tryb AP i STA mają osobne ścieżki inicjalizacji. `/wifi.txt` używa formatu JSON (`{"ssid": "...", "password": "..."}`).
