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
void changeBright_Temp(bool isBright)
{
    static int lastReadA = HIGH;
    int readA = digitalRead(ENC_A);
    if (readA != lastReadA)
    {
        if (digitalRead(ENC_B) != readA)
        {
            if (isBright)
            {
                brightness_value_v2 += 20;
                if (brightness_value_v2 > 1000)
                    brightness_value_v2 = 1000;
            }
            else
            {
                temperature_value_v2 += 20;
                if (temperature_value_v2 > 1000)
                    temperature_value_v2 = 1000;
            }
        }
        else
        {
            if (isBright)
            {
                brightness_value_v2 -= 20;

                if (brightness_value_v2 < 0)
                    brightness_value_v2 = 0;
            }
            else
            {
                temperature_value_v2 -= 20;

                if (temperature_value_v2 < 0)
                    temperature_value_v2 = 0;
            }
        }
        drawMenu();
    }

    lastReadA = readA;
}
void handleEncoder(){
    if (editingValue){
        if (getSelectedItem() == BRIGHTNESS)
            changeBright_Temp(true);
        if (getSelectedItem() == TEMPERATURE)
            changeBright_Temp(false);
        return;
    }
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
    sendLightState();
    drawMenu();
    delay(200); // debounce simples
    break;

    case MODE:
    ColorMode = !ColorMode;
    if(ColorMode){
        work_mode = "colour";
    }else {
        work_mode = "white";
    }
    sendLightState();
    drawMenu();
    delay(200);
    break;

    case COLOR:
    telaCor();
    sendLightState();
    drawMenu();
    break;

    case BRIGHTNESS:
    editingValue = !editingValue;
    sendLightState();
    drawMenu();
    break;

    case TEMPERATURE:
    editingValue = !editingValue;
    sendLightState();
    drawMenu();
    break;
    }
}
    lastButtonState = currentButtonState;
};

