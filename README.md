[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
    bodmer/TFT_eSPI
    bblanchon/ArduinoJson

[platformio]
description = Inicijalni testni projekt spajanja na wifi, konzumiranja free API-ja, prikaza na vanjskom displayu

**Preduvjet za kreiranje lokalnog image-a i uploadanje esp32 image na mikrokontroler je Secrets.h fajl sa ispravnim podacima.**
