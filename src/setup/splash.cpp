#include <Arduino.h>
#include "telaCores.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#include "setup.h"
#include "navMenu.h"
void showBootScreen() {
    tft.setSwapBytes(true);
    tft.pushImage(0, 0, 160, 128, Lighter_SplashScreen);
    delay (2000);
}