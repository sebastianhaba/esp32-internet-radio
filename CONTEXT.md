# ESP32 Internet Radio

Internetowe radio streamingowe oparte na Seeed XIAO ESP32S3 z wyświetlaczem ST7735 i enkoderem.

## Platforma

**XIAO ESP32S3**: Seeed Studio XIAO ESP32S3 — mikrokontroler z dwurdzeniowym ESP32-S3, 8MB Flash, PSRAM, WiFi 2.4 GHz.

## Język

**Stacja**:
Pojedyncze radio internetowe — para (nazwa, URL strumienia audio).
_Unikaj_: kanał, stream, preset

**Scena**:
Stan interfejsu użytkownika na wyświetlaczu — kontroluje co jest rysowane i jakie interakcje są dostępne.
_Unikaj_: ekran, widok, strona

**Tryb AP** (Access Point):
Tryb awaryjny/konfiguracyjny, w którym radio tworzy własną sieć WiFi, zamiast łączyć się z siecią domową. Służy do pierwszej konfiguracji połączenia WiFi.
_Unikaj_: tryb konfiguracyjny, hotspot

**Tryb STA** (Station):
Normalny tryb pracy — radio łączy się z domową siecią WiFi jako klient.
_Unikaj_: tryb normalny, tryb klienta

**Konfiguracja WiFi**:
Dane potrzebne do połączenia z siecią domową: SSID i hasło. Przechowywane w pliku `/wifi.txt` w formacie JSON na LittleFS.
_Unikaj_: credentials, secrets

## Relacje

- Jedna **Scena** jest aktywna w danym momencie — `SceneManager` zarządza przejściami między scenami
- **Tryb AP** jest uruchamiany tylko gdy brak konfiguracji WiFi lub połączenie się nie powiedzie
- **Tryb STA** wymaga poprawnej konfiguracji WiFi i udanego połączenia

## Przykładowy dialog

> **Dev:** "Co się stanie, jeśli user poda złe hasło do WiFi?"
> **Domain expert:** "Radio będzie próbować połączyć się przez 30 sekund. Jeśli się nie uda, przechodzi w tryb AP i wyświetla na LCD adres 192.168.100.1, pod który user ma wejść by podać poprawne dane."

## Flagged ambiguities

- "konfiguracja" była używana zarówno w kontekście konfiguracji WiFi, jak i konfiguracji listy stacji — rozróżnione jako oddzielne endpointy API
