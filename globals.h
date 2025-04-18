#pragma once
#include <Arduino.h>

struct IpConfig {
  byte ipByte;
};

extern byte heldKeys;
extern byte subnetByte;
extern short mode;
extern bool inModeSwitch;
extern unsigned long currentMillis;
extern unsigned long lastSend;

extern IpConfig ipConfigs[4];
extern byte currentIpIndex;

