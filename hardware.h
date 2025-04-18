#pragma once
#include <Adafruit_NeoKey_1x4.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3D

extern Adafruit_SSD1306 display;
extern Adafruit_MultiNeoKey1x4 neokey;

NeoKey1x4Callback press(keyEvent evt);

