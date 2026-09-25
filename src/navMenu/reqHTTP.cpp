#include <Arduino.h>
#include "telaCores.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#include "setup.h"
#include "navMenu.h"
bool lightState = false;
void sendLightState()
{
    HTTPClient http;
    http.begin(API_URL);
    http.addHeader("Content-Type", "application/json");
    String json = "{";
      json += "\"power\":" + String(lightState ? "true" : "false") + ",";
      json += "\"work_mode\":\"white\",";
      json += "\"brightness_value_v2\":1000,";
      json += "\"temperature_value_v2\":1000,";
      json += "\"color\":{";
      json += "\"h\":0,";
      json += "\"s\":0,";
      json += "\"v\":1000";
      json += "}";
      json += "}";
    int httpCode = http.POST(json);
    Serial.print("HTTP: ");
    Serial.println(httpCode);
    Serial.println(http.getString());
    http.end();
}