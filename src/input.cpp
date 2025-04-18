#include <Arduino.h>
#include "globals.h"
#include "input.h"

uint32_t press(keyEvent evt) {
  uint8_t key = evt.bit.NUM;

  // === KEY DOWN ===
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    heldKeys |= (1 << key);

    // Do absolutely nothing if only key 0 is held
    if ((heldKeys & 0x0F) == 0x01) return 0;

    // Handle key combos
    switch (heldKeys & 0x0F) {
      case 0x09:  // key 0 + key 3
        if (currentIpIndex > 0) currentIpIndex--;
        return 0;
      case 0x05:  // key 0 + key 2
        mode = 1 - mode;
        return 0;
      case 0x03:  // key 0 + key 1
        if (currentIpIndex < 3) currentIpIndex++;
        return 0;
      default:
        break;
    }

    // If key is beyond index keys and not locked by key 0:
    if (key > 1 && (heldKeys & 0x01) == 0) {
      if (mode == 1) {
        ipConfigs[currentIpIndex].ipByte ^= (1 << key);
      } else {
        int flipped = 7 - key;
        byte newMask = 0xFF << (8 - (flipped + 1));
        ipConfigs[currentIpIndex].subnetByte =
            (ipConfigs[currentIpIndex].subnetByte == newMask) ? 0x00 : newMask;
      }
    }

    return 0;
  }

  // === KEY UP ===
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    heldKeys &= ~(1 << key);

    // If no combo and only a single key 0/1 was pressed+released:
    if ((key == 0 || key == 1) && (heldKeys & 0x0F) == 0x00) {
      if (mode == 1) {
        ipConfigs[currentIpIndex].ipByte ^= (1 << key);
      } else {
        int flipped = 7 - key;
        byte proposed = 0xFF << (8 - (flipped + 1));
        ipConfigs[currentIpIndex].subnetByte =
            (ipConfigs[currentIpIndex].subnetByte == proposed) ? 0x00 : proposed;
      }
    }
  }

  return 0;
}
