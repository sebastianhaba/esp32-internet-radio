Oto pełny README w jednym bloku do skopiowania:

```markdown
# ESP32 Internet Radio / Radio Internetowe ESP32

---

## 🇵🇱 Polski

### Opis projektu

Proste radio internetowe oparte na module **Seeed Studio XIAO ESP32S3**.  
Projekt odgrywa strumień audio przez sieć Wi-Fi i wyświetla informacje o stacji oraz głośności na małym wyświetlaczu TFT.  
Aktualnie odgrywa jeden strumień: **Radio Nowy Świat** (`http://stream.nowyswiat.online/mp3`).

Sterowanie odbywa się za pomocą enkodera obrotowego:
- Obrót w prawo — zwiększenie głośności  
- Obrót w lewo — zmniejszenie głośności  
- Krótkie naciśnięcie — EV_PRESS (do rozbudowy)  
- Długie naciśnięcie — EV_LONG (do rozbudowy)

### Sprzęt

- [Seeed Studio XIAO ESP32S3](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)
- Wyświetlacz TFT ST7735 (160×128)
- Moduł DAC I2S (np. MAX98357A lub PCM5102)
- Enkoder obrotowy z przyciskiem

### Podłączenie pinów

| Peryferal  | Sygnał | Pin  |
|------------|--------|------|
| I2S DAC    | LRC    | D8   |
| I2S DAC    | BCLK   | D9   |
| I2S DAC    | DOUT   | D10  |
| Enkoder    | DT     | D7   |
| Enkoder    | CLK    | D6   |
| Enkoder    | SW     | D5   |
| TFT ST7735 | SCK    | D0   |
| TFT ST7735 | MOSI   | D1   |
| TFT ST7735 | DC     | D2   |
| TFT ST7735 | RST    | D3   |
| TFT ST7735 | CS     | D4   |

### Wymagania

#### Narzędzia

Projekt korzysta z **PlatformIO**. W VS Code należy zainstalować następujące rozszerzenie:

- [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

#### Plik `secrets.ini`

Przed kompilacją należy utworzyć plik `secrets.ini` w katalogu głównym projektu z następującą treścią:

```ini
[secrets]
build_flags =
    -D WIFI_SSID=\"nazwa_sieci\"
    -D WIFI_PASS=\"haslo_wifi\"
```

Plik ten nie jest śledzony przez Git — nie należy go udostępniać publicznie.

### Biblioteki

- [ESP32-audioI2S](https://github.com/schreibfaul1/ESP32-audioI2S) — odtwarzanie audio przez I2S
- [Adafruit ST7735 (fork bez SD)](https://github.com/sebastianhaba/Adafruit-ST7735-Library/tree/1.11.0-no-sd) — obsługa wyświetlacza TFT; użyto forka oryginalnej biblioteki Adafruit w celu usunięcia zależności od biblioteki SD

### Budowanie i wgrywanie

```sh
pio run --target upload
pio device monitor
```

---

## 🇬🇧 English

### Project Description

A simple internet radio based on the **Seeed Studio XIAO ESP32S3** module.  
The project streams audio over Wi-Fi and displays station info and volume level on a small TFT screen.  
Currently plays a single stream: **Radio Nowy Świat** (`http://stream.nowyswiat.online/mp3`).

Control is handled via a rotary encoder:
- Rotate right — increase volume  
- Rotate left — decrease volume  
- Short press — EV_PRESS (reserved for future use)  
- Long press — EV_LONG (reserved for future use)

### Hardware

- [Seeed Studio XIAO ESP32S3](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)
- ST7735 TFT display (160×128)
- I2S DAC module (e.g. MAX98357A or PCM5102)
- Rotary encoder with push button

### Pin Connections

| Peripheral | Signal | Pin  |
|------------|--------|------|
| I2S DAC    | LRC    | D8   |
| I2S DAC    | BCLK   | D9   |
| I2S DAC    | DOUT   | D10  |
| Encoder    | DT     | D7   |
| Encoder    | CLK    | D6   |
| Encoder    | SW     | D5   |
| TFT ST7735 | SCK    | D0   |
| TFT ST7735 | MOSI   | D1   |
| TFT ST7735 | DC     | D2   |
| TFT ST7735 | RST    | D3   |
| TFT ST7735 | CS     | D4   |

### Requirements

#### Tooling

This project uses **PlatformIO**. Install the following VS Code extension:

- [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

#### secrets.ini file

Before building, create a secrets.ini file in the project root with the following content:

```ini
[secrets]
build_flags =
    -D WIFI_SSID=\"your_network_name\"
    -D WIFI_PASS=\"your_wifi_password\"
```

This file is excluded from version control — do not share it publicly.

### Libraries

- [ESP32-audioI2S](https://github.com/schreibfaul1/ESP32-audioI2S) — audio playback via I2S
- [Adafruit ST7735 (no-SD fork)](https://github.com/sebastianhaba/Adafruit-ST7735-Library/tree/1.11.0-no-sd) — TFT display driver; a fork of the original Adafruit library with the SD dependency removed

### Build & Flash

```sh
pio run --target upload
pio device monitor
```
