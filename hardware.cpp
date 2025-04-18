// hardware.cpp (if needed)
#include "hardware.h"
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET);

Adafruit_NeoKey_1x4 nk_array[2] = {
  { Adafruit_NeoKey_1x4(0x30, &Wire1) },
  { Adafruit_NeoKey_1x4(0x31, &Wire1) },
};
Adafruit_MultiNeoKey1x4 neokey((Adafruit_NeoKey_1x4*)nk_array, 1, 2);

