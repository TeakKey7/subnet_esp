#include <Arduino.h>
#include "globals.h"
#include "hardware.h"

void updateNeoKeyLEDs() {
  for (int i = 0; i < 8; i++) {
    bool bitSet = (1 << i);

    if (heldKeys & bitSet) {
      neokey.setPixelColor(i, 0xFF0000); // Red = held
    } else if (
      (mode == 0 && (ipConfigs[currentIpIndex].subnetByte & bitSet)) ||
      (mode == 1 && (ipConfigs[currentIpIndex].ipByte & bitSet))
    ) {
      neokey.setPixelColor(i, (mode == 1) ? 0x0000FF : 0x00FF00); // Blue or green
    } else {
      neokey.setPixelColor(i, 0x000000); // Off
    }
  }

  neokey.show();
}
