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
bool lastButtonState = HIGH;
int lastEncoderA = HIGH;
void handleEncoder(){
int currentEncoderA = digitalRead(ENC_A);
    if (currentEncoderA != lastEncoderA) {
        if (digitalRead(ENC_B) != currentEncoderA) {
            selectedIndex++;
        } else {
            selectedIndex--;
        }
        int menuSize = getMenuSize();
        if (selectedIndex >= menuSize) {
            selectedIndex = 0;
        }
        if (selectedIndex < 0) {
            selectedIndex = menuSize - 1;
        }
        drawMenu();
    }

    lastEncoderA = currentEncoderA;
};

void handleButton(){
    bool currentButtonState = digitalRead(BUTTON_PIN);
    if (currentButtonState == LOW && lastButtonState == HIGH) {
    switch (getSelectedItem())
    {
    case POWER:
    lightState = !lightState;

    Serial.print("Luz: ");
    Serial.println(lightState ? "LIGADA" : "DESLIGADA");

    sendLightState();

    delay(200); // debounce simples
    break;
    case MODE:
    ColorMode = true;
    break;
    case COLOR:
    telaCor();
    break;
    }
}
    lastButtonState = currentButtonState;
};