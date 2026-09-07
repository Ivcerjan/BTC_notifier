#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

inline bool parseBitcoinPrice(const String &payload, float &outPrice)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
        return false;
    }

    if (!doc["bitcoin"]["eur"].is<float>())
    {
        return false;
    }

    outPrice = doc["bitcoin"]["eur"];
    return true;
}