#pragma once

#include <Arduino.h>
#include "util.h"
struct NetworkInfo {
  byte ip;
  byte subnet;
  byte networkAddr;
  byte broadcastAddr;
  byte usableMin;
  byte usableMax;
  byte cidrSuffix;
  byte totalHosts;
};

inline NetworkInfo computeNetworkInfo(byte ip, byte subnet) {
  NetworkInfo info;
  info.ip = ip;
  info.subnet = subnet;

  info.cidrSuffix = countBits(subnet);
  byte hostBits = 8 - info.cidrSuffix;
  info.totalHosts = (hostBits >= 2) ? (1 << hostBits) - 2 : 0;

  info.networkAddr = ip & subnet;
  info.broadcastAddr = info.networkAddr | (~subnet);

  if (info.totalHosts >= 2) {
    info.usableMin = info.networkAddr + 1;
    info.usableMax = info.broadcastAddr - 1;
  } else {
    info.usableMin = 0;
    info.usableMax = 0;
  }

  return info;
}

