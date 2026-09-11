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

Example of terminal output when uploaded to device and monitored:

<img width="875" height="326" alt="image" src="https://github.com/user-attachments/assets/16d6fdde-9e81-41a1-baff-d88c05713fb4" />

## Hardware

| Component | Model |
|---|---|
| Microcontroller | ESP32 Board (I use Devkit V1) |
| Display | 1.8" TFT LED display (I use GERUI 128x160 SPI with ST7735 driver) |

> Temporary unformatted pin combination: 
> VCC→3V3,
> GND→GND,
> CS→GPIO5,
> RST→GPIO4,
> DC→GPIO2,
> SDA→GPIO23,
> SCK→GPIO18,
> LED→3V3

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) (VSCode extension or CLI)
- ESP32 board and TFT display wired and configured for `TFT_eSPI`

### Setup

1. Clone the repository
```bash
   git clone https://github.com/Ivcerjan/BTC_notifier.git
   cd BTC_Notifier
```

2. Create your secrets file from the template:
```bash
   cp include/Secrets.example.h include/Secrets.h
```

3. Fill in your WiFi credentials and API URL in `include/Secrets.h`:
```cpp
   const char *WIFI_SSID = "your-ssid";
   const char *WIFI_PASSWORD = "your-password";
   const char *COINGECKO_API_URL = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=eur";
```

4. Build and upload:
```bash
   pio run --target upload

   or just use the PlatformIO extension for VSCode, cannot recommend it enough
```

5. Open the serial monitor to watch connection and price logs:
```bash
   pio device monitor
```

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

- [ ] Wiring diagram
- [ ] Alternative crypto tracking alongside BTC
- [ ] Button-based toggle between assets
- [ ] Price change indicator (colour-coded)

## License

MIT
