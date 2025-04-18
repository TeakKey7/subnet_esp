#pragma once

#include "network_info.h"

// These should be defined in main.ino or another shared header
extern byte subnetByte;
extern byte currentIpIndex;

inline void printStatus() {
  NetworkInfo info = computeNetworkInfo(ipConfigs[currentIpIndex].ipByte, subnetByte);

  Serial.print("{\"subnet\":");
  Serial.print(info.subnet);
  Serial.print(",\"ip\":[");
  for (int i = 0; i < 4; i++) {
    Serial.print(ipConfigs[i].ipByte);
    if (i < 3) Serial.print(",");
  }
  Serial.print("],\"current\":");
  Serial.print(currentIpIndex);
  Serial.print(",\"usable\":[");
  Serial.print(info.usableMin);
  Serial.print(",");
  Serial.print(info.usableMax);
  Serial.print("],\"hosts\":");
  Serial.print(info.totalHosts);
  Serial.print(",\"cidr\":");
  Serial.print(info.cidrSuffix + 24);
  Serial.println("}");
}

