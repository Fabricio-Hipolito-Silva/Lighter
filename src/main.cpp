#include <Arduino.h>
#include "telaCores.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#include "setup.h"
#include "navMenu.h"
#define BUTTON_PIN 15
#define ENC_A 19
#define ENC_B 18

void setup()
{
    Serial.begin(115200);
    iniciarTelaCor();
    connectWifi();
    pinMode(ENC_A, INPUT_PULLUP);
    pinMode(ENC_B, INPUT_PULLUP);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    drawMenu();
}
    
void loop()
{
    handleEncoder();
    handleButton();
}
