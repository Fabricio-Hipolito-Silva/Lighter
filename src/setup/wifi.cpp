#include <Arduino.h>
#include <WiFi.h>
#include "secret.h"
#include "setup.h"
void connectWifi(){
  WiFi.begin(SSID, PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("...");
  }
   Serial.println();
    Serial.println("Wi-Fi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}