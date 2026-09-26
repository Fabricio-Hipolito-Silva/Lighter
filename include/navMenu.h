#ifndef NAVMENU_H
#define NAVMENU_H
#include <TFT_eSPI.h>
extern int selectedIndex;
extern bool ColorMode;
extern bool lightState;
extern String work_mode;
extern int brightness_value_v2;
extern int temperature_value_v2;
extern int h; extern int s; extern int v;
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