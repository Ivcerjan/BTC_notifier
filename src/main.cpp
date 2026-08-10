#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include "Secrets.h"

TFT_eSPI tft = TFT_eSPI();

unsigned long lastUpdate = 0;
constexpr unsigned long UPDATE_INTERVAL_MS = 5000;

#pragma region Helper metode
void showMessage(const char *message)
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println(message);
}

void prepareScreen()
{
    tft.init();
    tft.setRotation(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    showMessage("Connecting WiFi...");
}

void connectWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");

    showMessage("Connected!");

    delay(1000);
}

void printPrice(float price)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Bitcoin (BTC)");

    tft.setTextSize(3);
    tft.setCursor(10, 50);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println(price, 2);

    Serial.print("BTC price: ");
    Serial.println(price);
}

void printHttpError(int httpCode)
{
    Serial.print("HTTP error: ");
    Serial.println(httpCode);
    showMessage("API error!");
}

bool fetchBitcoinPrice(float &outPrice)
{
    HTTPClient http;
    http.begin(COINGECKO_API_URL);
    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK)
    {
        printHttpError(httpCode);
        http.end();
        return false;
    }

    String payload = http.getString();
    http.end();

    Serial.println(payload);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
        Serial.println("Error parsing JSON");
        return false;
    }

    outPrice = doc["bitcoin"]["eur"];
    return true;
}
#pragma endregion

void setup()
{
    Serial.begin(115200);

    prepareScreen();
    connectWifi();
}

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - lastUpdate < UPDATE_INTERVAL_MS)
    {
        return;
    }

    lastUpdate = currentMillis;

    if (WiFi.status() != WL_CONNECTED)
    {
        showMessage("WiFi error!");
        return;
    }

    float price;
    if (fetchBitcoinPrice(price))
    {
        printPrice(price);
    }
}
