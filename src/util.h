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

