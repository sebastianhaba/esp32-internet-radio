# ESP32 Internet Radio

Internet radio built on ESP32-S3 (Seeed XIAO) with ST7735 160×128 display.

**Version:** 0.1.0

---

## Screenshot

![Radio in action](docs/1.gif)

---

## Features

- MP3/AAC internet stream playback
- Station selection screen with carousel (long press encoder)
- Horizontal scrolling for long song titles
- VU Meter — audio level visualization (L+R channel, 15 segments, green/yellow/red)
- Volume control via encoder
- Display: time (NTP), IP address, codec name, bitrate
- Support for stations without metadata (e.g. Polskie Radio Trójka)

---

## Adding your own stations

Edit the `STATIONS` array in `src/main.cpp`:

```cpp
Station STATIONS[] = {
    {"Radio Nowy Swiat", "http://stream.nowyswiat.online/mp3"},
    {"Radio 357",        "https://stream.radio357.pl/"},
    {"Your station",     "http://your-stream-url..."},
};
```

---

## Usage

| Action | Encoder |
|---|---|
| Rotate CW/CCW | Volume up/down |
| Long press | Toggle playing screen ↔ station list |
| Short press (in list) | Select station |
| Rotate (in list) | Browse stations (carousel) |

---

## Dependencies (PlatformIO)

```
schreibfaul1/ESP32-audioI2S@3.4.5
bodmer/TFT_eSPI@^2.5.43
```

---

## Pinout (Seeed XIAO ESP32S3)

| Function | Pin |
|---|---|---|
| **ST7735 Display** | |
| TFT SCLK | D8 |
| TFT MOSI | D10 |
| TFT DC | D2 |
| TFT RST | D3 |
| TFT CS | D4 |
| **I2S Amplifier** | |
| I2S LRC | D0 |
| I2S BCLK | D9 |
| I2S DOUT | D1 |
| **Encoder** | |
| ENC CLK | D6 |
| ENC DT | D7 |
| ENC SW | D5 |

---

## Building

```bash
pio run            # compile
pio run -t upload  # compile + flash
pio run -t monitor # Serial monitor
```

---

## License

MIT
