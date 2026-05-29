# ESP32 Internet Radio

Internetowe radio streamingowe oparte na Seeed XIAO ESP32S3 z wyświetlaczem ST7735 i enkoderem.

**Wersja:** 0.3.0

## Platforma

**XIAO ESP32S3**: Seeed Studio XIAO ESP32S3 — mikrokontroler z dwurdzeniowym ESP32-S3, 8MB Flash, PSRAM, WiFi 2.4 GHz.

## Architektura

Projekt używa **wzorca podsystemów** — jeden plik `main.cpp` (~120 linii) deleguje do wyspecjalizowanych klas:

| Klasa | Odpowiedzialność |
|---|---|
| `AudioSystem` | I2S, strumień audio, głośność, VU level |
| `RotarySystem` | Enkoder + przycisk, FreeRTOS task na rdzeniu 1, kolejka zdarzeń |
| `DisplaySystem` | TFT_eSPI + TFT_eSprite (double buffering), fonty, rysowanie |
| `StationManager` | Lista stacji, karuzela, wczytywanie z pliku/fallback |
| `SceneManager` | Przełączanie scen, delegowanie draw/event |
| `RadioWebServer` | Serwer HTTP na porcie 80, edycja stacji przez WWW |

**Sceny:** `PlayingScene` (odtwarzanie + VU meter + scroll tytułu), `StationListScene` (karuzela wyboru), `LoadingScene` (wczytywanie z timeoutem).

## Język domeny

**Stacja**:
Pojedyncze radio internetowe — para (nazwa, URL strumienia audio). Przechowywana w `stations.txt` na LittleFS, edytowalna przez WWW.
_Unikaj_: kanał, stream, preset

**Scena**:
Stan interfejsu użytkownika na wyświetlaczu — kontroluje co jest rysowane i jakie interakcje są dostępne.
_Unikaj_: ekran, widok, strona

**Karuzela** (scroll):
Poziome przewijanie tekstu. Dwie kopie tytułu na "taśmie" — gdy jedna znika z lewej, druga wchodzi z prawej. Bez nagłego skoku.
_Unikaj_: marquee

**Ladder** (VU Meter):
Wizualizacja poziomu dźwięku — rząd segmentów (prostokątów) dla każdego kanału. Aktywne segmenty w kolorze, nieaktywne z obrysem.
_Unikaj_: bargraph

## Decyzje architektoniczne

### 1. TFT_eSPI zamiast Adafruit GFX/ST7735
- Szybszy rendering (DMA na ESP32S3)
- Wbudowany TFT_eSprite do double bufferingu
- Piny konfigurowane przez `build_flags` w `platformio.ini`, nie w kodzie

### 2. Fonty jako C array (`.h`), nie `.vlw` z LittleFS
- ArchivoNarrowRegular 5pt (top bar, info bar) i 8pt (stream title)
- `.vlw` z LittleFS powodowało spadek wydajności (~58KB odczytu z FS na klatkę)
- C array = zero FS reads, font załadowany w PROGMEM

### 3. LittleFS tylko na dane konfiguracyjne
- `stations.txt` — lista stacji (format: `Nazwa|URL`, linie od `#` to komentarze)
- `index.html` — interfejs WWW
- Partycja 4MB, auto-format przy pierwszym uruchomieniu
- `uploadfs` do wgrywania plików

### 4. Konfiguracja stacji przez WWW
- Własny serwer HTTP (tylko WiFiServer/WiFiClient, bez zewnętrznych bibliotek)
- `GET /` → index.html, `GET /stations` → surowy plik, `POST /save` → zapis + reload
- Pełny URL decoder do poprawnego zapisywania znaków specjalnych

### 5. Wykrywanie połączenia dla stacji bez metadanych
- `evt_streamtitle` + `evt_name` (dla stacji z metadanymi)
- `isRunning()` jako safety net (dla stacji jak PR Trójka — zero metadanych)

### 6. VU Meter — Ladder Display
- 15 segmentów na kanał (8+2px), zielony/żółty/czerwony
- Dwa paski jeden pod drugim, bez labeli L/R
- Refresh ~30fps przez `needsContinuous` od `State::Connected`

## Relacje

- `SceneManager` zarządza przejściami między `PlayingScene`, `StationListScene`, `LoadingScene`
- `StationManager` ładuje stacje z LittleFS; przy braku pliku używa fallback'owych z kodu
- `AudioSystem::play()` czyści `_streamTitle` i ustawia `_titleChanged` — zapobiega wyświetlaniu starego tytułu
- `RadioWebServer` po zapisie woła `stationManager.loadFromFile()` — stacje aktualizowane bez restartu

## Zależności (PlatformIO)

```
schreibfaul1/ESP32-audioI2S@3.4.5  — strumieniowanie audio
bodmer/TFT_eSPI@^2.5.43            — wyświetlacz + sprite
```

## Piny (Seeed XIAO ESP32S3)

| Funkcja | Pin |
|---|---|
| TFT SCLK | D8 |
| TFT MOSI | D10 |
| TFT DC | D2 |
| TFT RST | D3 |
| TFT CS | D4 |
| I2S LRC | D0 |
| I2S BCLK | D9 |
| I2S DOUT | D1 |
| ENC CLK | D6 |
| ENC DT | D7 |
| ENC SW | D5 |
