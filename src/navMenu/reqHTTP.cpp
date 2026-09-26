#include <Arduino.h>
#include "telaCores.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#include "setup.h"
#include "navMenu.h"
bool lightState = false;
String work_mode = "white";
int brightness_value_v2 = 1000;
int temperature_value_v2 = 1000;
int h = 180; int s = 1000; int v = 1000;

void sendLightState()
{
    HTTPClient http;
    http.begin(API_URL);
    http.addHeader("Content-Type", "application/json");
    String json = "{";
    json += "\"power\":" + String(lightState ? "true" : "false") + ",";
    json += "\"work_mode\":\"" + work_mode + "\",";
    json += "\"brightness_value_v2\":" + String(brightness_value_v2) + ",";
    json += "\"temperature_value_v2\":" + String(temperature_value_v2) + ",";
    json += "\"color\":{";
    json += "\"h\":" + String(h) + ",";
    json += "\"s\":" + String(s/10) + ",";
    json += "\"v\":" + String(v/10);
    json += "}";
    json += "}";
    int httpCode = http.POST(json);
    Serial.print("HTTP: ");
    Serial.println(httpCode);
    Serial.println(http.getString());
    http.end();
}