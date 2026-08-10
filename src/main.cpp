#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include "Secrets.h"

TFT_eSPI tft = TFT_eSPI();

unsigned long lastUpdate = 0;
const long interval = 5000; // ms

#pragma region Helper metode
void prepareScreen()
{
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Spajam na WiFi...");
}

void connectWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi spojen!");

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("Povezano!");

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
    tft.print("E");
    tft.println(price, 2);

    Serial.print("BTC cijena: ");
    Serial.println(price);
}

void printHttpError(int httpCode)
{
    Serial.print("HTTP greska: ");
    Serial.println(httpCode);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("API greska!");
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

    if (currentMillis - lastUpdate < interval)
    {
        return;
    }

    lastUpdate = currentMillis;

    if (WiFi.status() != WL_CONNECTED)
    {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(10, 10);
        tft.println("WiFi greska!");
        return;
    }

    HTTPClient http;
    http.begin(COINGECKO_API_URL);
    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK)
    {
        printHttpError(httpCode);
        http.end();
        return;
    }

    String payload = http.getString();
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    Serial.println(payload);

    if (error)
    {
        Serial.println("Greska pri parsiranju JSON-a");

        http.end();
        return;
    }

    printPrice(doc["bitcoin"]["eur"]);

    http.end();
}
