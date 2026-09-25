#ifndef NAVMENU_H
#define NAVMENU_H
#include <TFT_eSPI.h>
extern int selectedIndex;
extern bool ColorMode;
extern bool lightState;
enum MenuItem {
    POWER,
    MODE,
    COLOR,
    BRIGHTNESS,
    TEMPERATURE
};
int getMenuSize();
void drawMenu();
MenuItem getSelectedItem();
void sendLightState();
void handleButton();
void handleEncoder();


#endif