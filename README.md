# BTC Notifier

A small ESP32-based device that fetches the live Bitcoin price from the CoinGecko API and displays it on a TFT screen, refreshing every few seconds.

![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![Framework](https://img.shields.io/badge/framework-Arduino-teal)
![Build](https://img.shields.io/badge/build-PlatformIO-orange)

## Features

- Live BTC price (EUR) via the [CoinGecko API](https://www.coingecko.com/en/api)
- TFT display output with error handling (WiFi and API failures shown on screen)
- Non-blocking update loop using `millis()` timing
- WiFi credentials kept out of source control via a local secrets file
- Foolproof wiring diagram for the exact model

Example of terminal output when uploaded to device and monitored:

<img width="875" height="326" alt="image" src="https://github.com/user-attachments/assets/16d6fdde-9e81-41a1-baff-d88c05713fb4" />

## Hardware

| Component | Model |
|---|---|
| Microcontroller | ESP32 Board (I use [Devkit V1](https://www.amazon.com/ELEGOO-ESP-WROOM-32-Development-Bluetooth-Microcontroller/dp/B0D8T53CQ5/ref=sr_1_1?crid=3RLZ78N1M2Y4D&dib=eyJ2IjoiMSJ9.w298Scd5FCHe-TUhyW0R1AiZP91zZcCXrSUvoVnx-NvG9vtfzIKKrRw8Y_qpenY7TqQ6KFW3SBN1CmOVTiMmrkjTJyYMAhq0d1wKi37iKEwK3zFEEONCNvOo8HawYSYOJ0iBhDO13v3jMxaYuFMLkRiAd8DYhxVMS37Kmas59q_Z-TFJLRlyll_OltvjJqRYyC-c7bENgGV7ZDJF1KedoFj5Eu1cbQgdKQDPF__h0ds.p4K7KVZTX21L0r9H2HoTiUnkS0nPFlM8UeZVKqoBrPs&dib_tag=se&keywords=elegoo+esp32&qid=1786086764&sprefix=elegoo+esp32%2Caps%2C213&sr=8-1)) |
| Display | 1.8" TFT LED display (I use [GERUI 128x160 SPI with ST7735 driver](https://www.amazon.de/-/en/GERUI-Display-Module-Definition-Colour/dp/B0D52SWXB4/ref=sr_1_3?crid=26KRPM57ILKHR&dib=eyJ2IjoiMSJ9.7HJ55FXEjqCnsMnQnGX9fx4OFgJ4RsDY5x6HBcwZSlYgRhKr9YLqY7kmtuTWSqcq5tt5F5sKQ4aoZjHdBGwGLCUMGo8r2ec-ioA8hWyz743VvqXgPA0mFNubDP9pjNpqHteYGIofqYOWIQioifa-d2BjYBCPPBVvsKGMdfYmj2uwSxA5xrmKeCh1e3WRmhy5eDeAi2AUEpsE1Q2oCqRV4V9isHbMsu6ySNm3JEgPuY2bHaiEUjVDIXUkCAfmSVfHtHHpKBI-qMFfHiwZF4KkvWtC0YNNh6hHKh4psCt0WZk.Dr4SdgcyRxRw2KO4ZCJNyUDU_OYeuBOtfCef5gFhwsk&dib_tag=se&keywords=HiLetgo+1.8%22+TFT+displej+%28ST7735%29&qid=1786091929&sprefix=hiletgo+1.8%22+tft+displej+st7735+%2Caps%2C128&sr=8-3)) |

## Wiring diagram

| Display | ESP32 |
|---|---|
| GND | GND |
| VCC | 3V3 |
| SCL | D18 |
| SDA | D23 |
| RES | D4 |
| DC | D2 |
| CS | D5 |
| BLK | 3V3 |

> Wiring uses the ESP32's VSPI bus (`SCK=18`, `MOSI=23`). Any free GPIO works for `CS`/`RES`/`DC` as long as it matches the `TFT_eSPI` build flags in `platformio.ini`. Reference: [ESP32-WROOM-32 datasheet](https://www.mouser.com/datasheet/2/891/esp-wroom-32_datasheet_en-1223836.pdf).

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) (VSCode extension or CLI)
- ESP32 board and TFT display wired and configured for `TFT_eSPI`

### Setup

1. Clone the repository
2. Create your Secrets.h file in the `/include` folder from the template Secrets.example.h
3. Fill in your WiFi credentials and API URL in `include/Secrets.h`, mainly the WIFI_SSID  and WIFI_PASSWORD
4. Build, upload, open the serial monitor:
```bash
   pio run --target upload
   pio device monitor
```
   or just use the PlatformIO extension for VSCode, cannot recommend it enough

## Finally
Hook up your ESP32 to the breadboard and follow the wiring diagram. Run the commands from the setup and you should see it on your TFT display!

## Project Structure

```
BTC_Notifier/
├── include/
│   ├── BitcoinParser.h     # Excluded reused method for deserialization of API response
│   ├── Config.h            # High level configuration, update intervals, timeouts, mock flags
│   ├── Secrets.example.h   # Template for WiFi credentials & API URL
│   └── Secrets.h           # Your actual credentials (gitignored, not committed)
├── src/
│   └── main.cpp            # Main application logic
├── test/
│   └── test_main.cpp       # Prebuild tests for parsing, missing fields, malformed JSON
├── platformio.ini          # PlatformIO project configuration
├── README.md
├── .gitignore
└── .vscode
    └── extensions.json     # PIO recommendation, unwanted arduino, etc
```


## Roadmap

- [ ] Alternative crypto tracking alongside BTC
- [ ] Button-based toggle between assets

## License

MIT
