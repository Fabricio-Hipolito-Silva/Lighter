#include "telaCores.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <math.h>

TFT_eSPI tft = TFT_eSPI();
#define CX 55
#define CY 64
#define RADIUS 50

// ==============================
// ENCODER
// ==============================

#define ENC_A 19
#define ENC_B 18
#define ENC_BUTTON 15

int lastA = HIGH;
bool editingHue = true;
// ==============================
// HSV
// ==============================
int hue = 0;
int saturation = 1000;
int brightness = 1000;
// ==============================
// HSV -> RGB565
// ==============================

uint16_t hsvToRGB565(float h, float s, float v)
{
    float r, g, b;
    h = fmod(h, 360.0f);
    s = constrain(s, 0.0f, 1.0f);
    v = constrain(v, 0.0f, 1.0f);
    float c = v * s;
    float x = c *
              (1.0f -
               fabs(fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;
    if (h < 60) {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 120) {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 180) {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 240) {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 300) {
        r = x;
        g = 0;
        b = c;
    }
    else {
        r = c;
        g = 0;
        b = x;
    }

    uint8_t R = (r + m) * 255;
    uint8_t G = (g + m) * 255;
    uint8_t B = (b + m) * 255;

    return tft.color565(R, G, B);
}
// ==============================
// DESENHA RODA
// ==============================
void drawColorWheel()
{
    for (int y = CY - RADIUS; y <= CY + RADIUS; y++) {
        for (int x = CX - RADIUS; x <= CX + RADIUS; x++) {
            int dx = x - CX;
            int dy = y - CY;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= RADIUS) {
                float angle =
                    atan2(dy, dx) * 180.0f / PI;
                if (angle < 0)
                    angle += 360.0f;
                float sat =
                    distance / RADIUS;
                uint16_t color =
                    hsvToRGB565(
                        angle,
                        sat,
                        1.0f
                    );
                tft.drawPixel(
                    x,
                    y,
                    color
                );
            }
        }
    }
}
// ==============================
// POSIÇÃO DO CURSOR
// ==============================
void drawCursor()
{
    float distance =
        (saturation / 1000.0f) * RADIUS;
    float angle =
        hue * PI / 180.0f;
    int cursorX =
        CX + cos(angle) * distance;
    int cursorY =
        CY + sin(angle) * distance;
    // círculo branco
    tft.drawCircle(
        cursorX,
        cursorY,
        4,
        TFT_WHITE
    );
    // centro preto para ficar visível
    tft.drawCircle(
        cursorX,
        cursorY,
        2,
        TFT_BLACK
    );
}
// ==============================
// INTERFACE
// ==============================
void drawInterface()
{
    // Fundo
    tft.fillScreen(TFT_BLACK);
    // Título
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString(
        "COR",
        118,
        5
    );
    // Roda
    drawColorWheel();
    // Cursor
    drawCursor();
    // ==========================
    // HUE
    // ==========================
    if (editingHue)
        tft.setTextColor(TFT_WHITE);
    else
        tft.setTextColor(TFT_DARKGREY);
    tft.setTextSize(1);
    tft.drawString(
        "HUE",
        115,
        35
    );
    if (editingHue)
        tft.drawCircle(
            145,
            39,
            3,
            TFT_WHITE
        );
    else
        tft.drawCircle(
            145,
            39,
            3,
            TFT_DARKGREY
        );
    // Valor do HUE
    tft.drawNumber(
        hue,
        115,
        48
    );
    tft.drawString(
        "deg",
        140,
        48
    );
    // ==========================
    // SAT
    // ==========================
    if (!editingHue)
        tft.setTextColor(TFT_WHITE);
    else
        tft.setTextColor(TFT_DARKGREY);
    tft.drawString(
        "SAT",
        115,
        65
    );
    if (!editingHue)
        tft.drawCircle(
            145,
            69,
            3,
            TFT_WHITE
        );
    else
        tft.drawCircle(
            145,
            69,
            3,
            TFT_DARKGREY
        );
    // Valor SAT
    tft.drawNumber(
        saturation,
        115,
        78
    );
    // ==========================
    // BRILHO
    // ==========================
    tft.setTextColor(TFT_WHITE);
    tft.drawString(
        "BRILHO",
        115,
        96
    );
    tft.drawNumber(
        brightness,
        115,
        109
    );
}
// ==============================
// LÊ ENCODER
// ==============================
void readEncoder()
{
    int currentA =
        digitalRead(ENC_A);
    if (currentA != lastA) {
        if (currentA == LOW) {
            int currentB =
                digitalRead(ENC_B);
            if (currentB != currentA) {
                // ==================
                // SENTIDO HORÁRIO
                // ==================
                if (editingHue) {
                    hue += 20;
                    if (hue >= 360)
                        hue = 0;
                }
                else {
                    saturation += 200;
                    if (saturation > 1000)
                        saturation = 1000;
                }
            }
            else {
                // ==================
                // SENTIDO ANTI-HORÁRIO
                // ==================
                if (editingHue) {
                    hue -= 20;
                    if (hue < 0)
                        hue = 355;
                }
                else {
                    saturation -= 200;
                    if (saturation < 0)
                        saturation = 0;
                }
            }

            drawInterface();
        }

        lastA = currentA;
    }
}


// ==============================
// BOTÃO
// ==============================

void readButton()
{
    static bool lastButton = HIGH;
    bool currentButton =
        digitalRead(ENC_BUTTON);
    if (lastButton == HIGH &&
        currentButton == LOW) {
        editingHue = !editingHue;
        drawInterface();
        delay(150);
    }
    lastButton = currentButton;
}
void telaCor()
{
    drawInterface();

    while (true)
    {
        readEncoder();
        readButton();
    }
}

void iniciarTelaCor()
{
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    drawInterface();

    pinMode(ENC_A, INPUT_PULLUP);
    pinMode(ENC_B, INPUT_PULLUP);
    pinMode(ENC_BUTTON, INPUT_PULLUP);
}