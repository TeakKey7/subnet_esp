#pragma once

#include <Arduino.h>

extern byte heldKeys;
extern short mode;
extern byte currentIpIndex;
extern unsigned long currentMillis;
extern unsigned long lastSend;

struct IpConfig {
  byte ipByte;
  byte subnetByte;
};

extern IpConfig ipConfigs[4];