#include <Arduino.h>
#include "globals.h"
#include "hardware.h"

void updateNeoKeyLEDs() {
  byte subnetBits = ipConfigs[currentIpIndex].subnetByte;
  byte ipBits = ipConfigs[currentIpIndex].ipByte;

  for (int i = 0; i < 8; i++) {
    uint8_t bit = (1 << i);

    if (heldKeys & bit) {
      neokey.setPixelColor(i, 0xFF0000); // Held key = red
    }
    else if (mode == 0 && (subnetBits & bit)) {
      neokey.setPixelColor(i, 0x00FF00); // Subnet = green
    }
    else if (mode == 1 && (ipBits & bit)) {
      neokey.setPixelColor(i, 0x0000FF); // IP = blue
    }
    else {
      neokey.setPixelColor(i, 0x000000); // Off
    }
  }

  neokey.show();
}
