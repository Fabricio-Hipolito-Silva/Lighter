#include <Arduino.h>
#include "telaCores.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#include "setup.h"
#include "navMenu.h"
bool ColorMode = false;
bool editingValue = false;
int getMenuSize(){
    if (ColorMode){
        return 5;
    }
    return 4;
};
int selectedIndex = 0;
const char* getMenuItem(int index, bool ColorMode, bool lightState){
    if(ColorMode){
        switch (index)
        {
        case 0: return lightState ? "Ligado" : "Desligado";
        case 1: return "Modo: Colorido";
        case 2: return "Mudar Cor";
        case 3: return "Brilho";
        case 4: return "Temperatura";
        }
    } else{
        switch (index)
        {
        case 0: return lightState ? "Ligado" : "Desligado";
        case 1: return "Modo: Branco";
        case 2: return "Brilho";
        case 3: return "Temperatura";
        }
    }
    return "";
}
void drawMenu() {
    tft.fillScreen(TFT_BLACK);
    int menuSize = getMenuSize();
    int itemHeight = 25;
    int startY = 10;
    for (int i = 0; i < menuSize; i++) {
        if (i == selectedIndex && !editingValue) {
            tft.setTextColor(TFT_BLACK, TFT_WHITE);
        } else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }
        tft.drawString(
            getMenuItem(i, ColorMode, lightState),
            5,
            startY + (i * itemHeight),
            2
        );
        if (ColorMode && i == BRIGHTNESS || !ColorMode && i == BRIGHTNESS -1){
            if (editingValue && getSelectedItem() == BRIGHTNESS)
            {
                tft.setTextColor(TFT_BLACK, TFT_WHITE);
            }else{
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
            }
            tft.drawString(
                String(brightness_value_v2/10)+ "%",
                110,
                startY + (i*itemHeight),
                2
            );
        }
        if (ColorMode && i == TEMPERATURE || !ColorMode && i == TEMPERATURE-1){
            if (editingValue && getSelectedItem() == TEMPERATURE)
            {
                tft.setTextColor(TFT_BLACK, TFT_WHITE);
            }else{
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
            }
            tft.drawString(
                String(temperature_value_v2/10)+ "%",
                110,
                startY + (i*itemHeight),
                2
            );
        }
    }

}
MenuItem getSelectedItem(){
    if(ColorMode){
        switch (selectedIndex)
        {
        case 0: return POWER;
        case 1: return MODE;
        case 2: return COLOR;
        case 3: return BRIGHTNESS;
        case 4: return TEMPERATURE;
        }
    } else{
        switch (selectedIndex)
        {
        case 0: return POWER;
        case 1: return MODE;
        case 2: return BRIGHTNESS;
        case 3: return TEMPERATURE;
        }
    }
    }
