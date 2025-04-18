// globals.cpp
#include <Arduino.h>
#include "globals.h"  // <-- This brings in the definition of IpConfig

byte heldKeys = 0;
byte subnetByte = 0;
byte ipByte = 0;
short mode = 0;
bool inModeSwitch = false;

unsigned long currentMillis = 0;
unsigned long lastSend = 0;

IpConfig ipConfigs[4] = {
    {42, 0}, {69, 0}, {13, 0}, {37, 0}
  };
  byte currentIpIndex = 0;
  