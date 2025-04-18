#include <Arduino.h>
#include "globals.h"
#include "input.h"

uint32_t press(keyEvent evt) {
  uint8_t key = evt.bit.NUM;

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    heldKeys |= (1 << key);

    if (!inModeSwitch && (heldKeys & 0b11) == 0b11) {
      mode = 1 - mode;
      inModeSwitch = true;
      return 0;
    }

    if (key > 1) {
      if (mode == 1) {
        ipConfigs[currentIpIndex].ipByte ^= (1 << key);  // ✅ FIXED
      } else {
        int flipped = 7 - key;
        byte newMask = 0xFF << (8 - (flipped + 1));
        subnetByte = (subnetByte == newMask) ? 0x00 : newMask;
      }
    }

    return 0;
  }

  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    heldKeys &= ~(1 << key);

    if (inModeSwitch && (heldKeys & 0b11) == 0) {
      inModeSwitch = false;
      return 0;
    }

    if ((key == 0 || key == 1) && !inModeSwitch) {
      if (mode == 1) {
        ipConfigs[currentIpIndex].ipByte ^= (1 << key);  // ✅ FIXED
      } else {
        int flipped = 7 - key;
        byte proposed = 0xFF << (8 - (flipped + 1));
        subnetByte = (subnetByte == proposed) ? 0x00 : proposed;
      }
    }
  }

  return 0;
}
