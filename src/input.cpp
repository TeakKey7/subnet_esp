#include <Arduino.h>
#include "globals.h"
#include "input.h"

uint32_t press(keyEvent evt) {
  uint8_t key = evt.bit.NUM;

  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    heldKeys |= (1 << key);

    // Custom heldKeys combo behavior
    switch (heldKeys & 0x0F) {
      case 0x01:
        // wait - do nothing
        break;
      case 0x09:
        if (currentIpIndex > 0) currentIpIndex--;
        break;
      case 0x05:
        mode = 1 - mode;
        break;
      case 0x03:
        if (currentIpIndex < 3) currentIpIndex++;
        break;
      default:
        break;
    }

    // Normal per-bit update (keys 2-7)
    if (key > 1) {
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

  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    heldKeys &= ~(1 << key);

    // Keys 0/1 single press fallbacks
    if ((key == 0 || key == 1) && ((heldKeys & 0x0F) == 0x00)) {
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
