#include "hardware.h"
#include "globals.h"
#include "network_info.h"
#include "print_status.h"
#include "display.h"
#include "util.h"
#include "json_comm.h"
#include <Arduino_JSON.h>

void setup() {
  Serial.begin(115200);
  if (!neokey.begin()) while (1) delay(10);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  for (uint16_t i = 0; i < 8; i++) {
    neokey.setPixelColor(i, 0x808080); neokey.show(); delay(50);
  }
  for (uint16_t i = 0; i < 8; i++) {
    neokey.setPixelColor(i, 0x000000); neokey.show(); delay(50);
  }

  for (int i = 0; i < 8; i++) {
    neokey.registerCallback(i, press);
  }
}

void loop() {
  readCommand();
  currentMillis = millis();
  neokey.read();

  for (int i = 0; i < 8; i++) {
    if (heldKeys & (1 << i)) {
      neokey.setPixelColor(i, 0xFF0000);
    } else if ((mode == 0 && (subnetByte & (1 << i))) ||
               (mode == 1 && (ipConfigs[currentIpIndex].ipByte & (1 << i)))) {
      neokey.setPixelColor(i, (mode == 1) ? 0x0000FF : 0x00FF00);
    } else {
      neokey.setPixelColor(i, 0x000000);
    }
  }
  neokey.show();

  if (millis() - lastSend >= 500) {
    printStatus();
    lastSend = millis();
  }
  updateDisplay();
  delay(10);
}