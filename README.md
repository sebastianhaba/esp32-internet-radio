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

---

## Jak dodać własne stacje

Edytuj tablicę `STATIONS` w `src/main.cpp`:

```cpp
Station STATIONS[] = {
    {"Radio Nowy Swiat", "http://stream.nowyswiat.online/mp3"},
    {"Radio 357",        "https://stream.radio357.pl/"},
    {"Twoja stacja",     "http://url-stacji..."},
};
```

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
pio run            # kompilacja
pio run -t upload  # kompilacja + flash
pio run -t monitor # Serial monitor
```

---

## Licencja

MIT
