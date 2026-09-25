#include <Arduino.h>
#include "telaCores.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "secret.h"
#define BUTTON_PIN 15
#define ENC_A 19
#define ENC_B 18
bool lightState = false;
bool lastButtonState = HIGH;
int lastEncoderA = HIGH;
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

enum MenuItem{
    POWER,
    MODE,
    COLOR,
    BRIGHTNESS,
    TEMPERATURE
};

bool ColorMode = false;
int getMenuSize(){
    if (ColorMode){
        return 5;
    }
    return 4;
};
int selectedIndex = 0;
const char* getMenuItem(int index, bool ColorMode){
    if(ColorMode){
        switch (index)
        {
        case 0: return "Ligado";
        case 1: return "Modo: Colorido";
        case 2: return "Mudar Cor";
        case 3: return "Brilho";
        case 4: return "Temperatura";
        }
    } else{
        switch (index)
        {
        case 0: return "Ligado";
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

        if (i == selectedIndex) {
            tft.setTextColor(TFT_BLACK, TFT_WHITE);
        } else {
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }

        tft.drawString(
            getMenuItem(i, ColorMode),
            5,
            startY + (i * itemHeight),
            2
        );
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
void sendLightState()
{
    HTTPClient http;
    http.begin(API_URL);
    http.addHeader("Content-Type", "application/json");
    String json = "{";
      json += "\"power\":" + String(lightState ? "true" : "false") + ",";
      json += "\"work_mode\":\"white\",";
      json += "\"brightness_value_v2\":1000,";
      json += "\"temperature_value_v2\":1000,";
      json += "\"color\":{";
      json += "\"h\":0,";
      json += "\"s\":0,";
      json += "\"v\":1000";
      json += "}";
      json += "}";
    int httpCode = http.POST(json);
    Serial.print("HTTP: ");
    Serial.println(httpCode);
    Serial.println(http.getString());
    http.end();
}
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
