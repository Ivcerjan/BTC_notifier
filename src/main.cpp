#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include "Secrets.h"
#include "BitcoinParser.h"
#include "Config.h"

TFT_eSPI tft = TFT_eSPI();

unsigned long lastUpdate = 0;
uint8_t consecutiveFailures = 0;

#define PROGRESS_BAR_Y      115 
#define PROGRESS_BAR_HEIGHT 6
#define PROGRESS_BAR_MARGIN 10

int lastProgressWidth = -1;

#pragma region Helper metode
void showMessage(const char *message)
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println(message);
    lastProgressWidth = -1;
}

void prepareScreen()
{
    tft.init();
    tft.setRotation(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    showMessage("Connecting WiFi...");
}

void drawProgressBar(unsigned long elapsed, unsigned long total)
{
    int barWidth = tft.width() - (PROGRESS_BAR_MARGIN * 2);
    int fillWidth = (int)((float)elapsed / total * barWidth);

    if (fillWidth == lastProgressWidth)
    {
        return;
    }
    lastProgressWidth = fillWidth;

    tft.drawRect(PROGRESS_BAR_MARGIN, PROGRESS_BAR_Y, barWidth, PROGRESS_BAR_HEIGHT, TFT_DARKGREY);

    tft.fillRect(PROGRESS_BAR_MARGIN + 1, PROGRESS_BAR_Y + 1, fillWidth - 2, PROGRESS_BAR_HEIGHT - 2, TFT_GREEN);
}

bool connectWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - start > WIFI_CONNECT_TIMEOUT_MS)
        {
            Serial.println("\nWiFi connect timed out!");
            showMessage("WiFi timeout!");
            return false;
        }

        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    showMessage("Connected!");
    delay(1000);

    return true;
}

void printPrice(float price)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 10);
    tft.println("Bitcoin BTC");

    tft.setTextSize(3);
    tft.setCursor(10, 50);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println(price, 2);

    lastProgressWidth = -1; // force redraw of bar next tick

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
    if (USE_MOCK_DATA)
    {
        outPrice = MOCK_BTC_PRICE;
        if (VERBOSE_LOGGING)
        {
            Serial.println("Using mock BTC price");
        }
        return true;
    }

    HTTPClient http;
    http.setTimeout(HTTP_TIMEOUT_MS);
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

    if (VERBOSE_LOGGING)
    {
        Serial.println(payload);
    }

    return parseBitcoinPrice(payload, outPrice);
}
#pragma endregion

void setup()
{
    Serial.begin(115200);

    prepareScreen();

    if (!connectWifi())
    {
    }
}

void loop()
{
    unsigned long currentMillis = millis();
    unsigned long elapsed = currentMillis - lastUpdate;

    // Update progress bar every tick, regardless of fetch timing
    if (WiFi.status() == WL_CONNECTED)
    {
        drawProgressBar(elapsed, UPDATE_INTERVAL_MS);
    }

    if (elapsed < UPDATE_INTERVAL_MS)
    {
        delay(50); // small tick so the bar animates smoothly without hammering the CPU
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
        consecutiveFailures = 0;
    }
    else
    {
        consecutiveFailures++;

        if (consecutiveFailures >= MAX_FETCH_RETRIES)
        {
            showMessage("Persistent API errors");
        }

        delay(RETRY_BACKOFF_MS);
    }
}