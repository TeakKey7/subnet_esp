#pragma once
#include <Arduino.h>

inline int countBits(uint8_t byte) {
  int count = 0;
  while (byte) {
    count += (byte & 1);
    byte >>= 1;
  }
  return count;
}

inline String byteToBinaryString(byte b) {
  String out = "";
  for (int i = 7; i >= 0; i--) {
    out += (b & (1 << i)) ? '1' : '0';
  }
  return out;
}


inline bool isValidSubnet(byte value) {
  const byte valid[] = {0, 128, 192, 224, 240, 248, 252};  // /25 to /30
  for (byte i = 0; i < sizeof(valid); i++) {
    if (value == valid[i]) return true;
  }
  return false;
}
