# ESP32 Internet Radio

Internetowe radio na ESP32-S3 (Seeed XIAO) z wyświetlaczem ST7735 160×128.

**Wersja:** 0.1.0

---

## Zrzut ekranu

![Radio w akcji](docs/1.gif)

---

## Funkcje

- Odtwarzanie strumieni MP3/AAC z internetu
- Ekran wyboru stacji z karuzelą (długie przytrzymanie enkodera)
- Poziome przewijanie długich tytułów utworów
- VU Meter — wizualizacja poziomu dźwięku (L+P kanał, 15 segmentów, zielony/żółty/czerwony)
- Regulacja głośności enkoderem
- Wyświetlanie czasu (NTP), adresu IP, kodeka i bitrate
 - Obsługa stacji bez metadanych (np. Polskie Radio Trójka)
 - Stacje konfigurowane przez plik `data/stations.txt` na LittleFS — bez rekompilacji
 - Konfiguracja stacji przez interfejs WWW — edytor w przeglądarce, zapis bez restartu

---

## Jak dodać własne stacje

Edytuj plik `data/stations.txt`:

```
# Format: Nazwa stacji|URL strumienia
Radio Nowy Swiat|http://stream.nowyswiat.online/mp3
Radio 357|https://stream.radio357.pl/
Twoja stacja|http://url-stacji...
```

Linie zaczynające się od `#` to komentarze. Po edycji wgraj plik na LittleFS:

```bash
pio run -t uploadfs
```

Jeśli plik `stations.txt` nie istnieje, radio użyje wbudowanych stacji zapasowych.

### Edycja stacji przez WWW

Po połączeniu z WiFi otwórz w przeglądarce adres IP radia (widoczny na dole ekranu i w Serial Monitorze). Strona umożliwia edycję listy stacji — po kliknięciu **Save & Reload** lista jest natychmiast aktualizowana bez restartu urządzenia.

---

## Jak używać

| Akcja | Enkoder |
|---|---|
| Obrót CW/CCW | Zwiększ/zmniejsz głośność |
| Długie przytrzymanie | Przełącz ekran odtwarzania ↔ lista stacji |
| Krótkie wciśnięcie (na liście) | Wybierz stację |
| Obrót (na liście) | Przeglądanie stacji (karuzela) |

---

## Zależności (PlatformIO)

```
schreibfaul1/ESP32-audioI2S@3.4.5
bodmer/TFT_eSPI@^2.5.43
```

---

## Piny (Seeed XIAO ESP32S3)

| Funkcja | Pin |
|---|---|---|
| **Wyświetlacz ST7735** | |
| TFT SCLK | D8 |
| TFT MOSI | D10 |
| TFT DC | D2 |
| TFT RST | D3 |
| TFT CS | D4 |
| **Wzmacniacz I2S** | |
| I2S LRC | D0 |
| I2S BCLK | D9 |
| I2S DOUT | D1 |
| **Enkoder** | |
| ENC CLK | D6 |
| ENC DT | D7 |
| ENC SW | D5 |

---

## Budowanie

```bash
pio run             # kompilacja
pio run -t upload   # kompilacja + flash
pio run -t uploadfs # wgranie plików z data/ na LittleFS
pio run -t monitor  # Serial monitor
```

Przy pierwszym uruchomieniu LittleFS zostanie automatycznie sformatowany. Pliki z katalogu `data/` wgrywane są na partycję LittleFS (4 MB).

---

## Licencja

MIT
