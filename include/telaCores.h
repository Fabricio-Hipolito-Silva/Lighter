#ifndef TELACORES_H
#define TELACORES_H
#include <TFT_eSPI.h>
extern int hue;
extern int saturation;
extern int brightness;
extern TFT_eSPI tft;

void telaCor();
void iniciarTelaCor();
#endif