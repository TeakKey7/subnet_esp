#include <Arduino.h>
#include <Arduino_JSON.h>
#include "network_info.h"
#include "globals.h"

void printStatus() {
  NetworkInfo info = computeNetworkInfo(ipConfigs[currentIpIndex].ipByte, subnetByte);

  JSONVar root;
  root["subnet"] = info.subnet;

  JSONVar ipArray;
  for (int i = 0; i < 4; i++) ipArray[i] = ipConfigs[i].ipByte;
  root["ip"] = ipArray;

  root["current"] = currentIpIndex;

  JSONVar usable;
  usable[0] = info.usableMin;
  usable[1] = info.usableMax;
  root["usable"] = usable;

  root["hosts"] = info.totalHosts;
  root["cidr"] = info.cidrSuffix + 24;

  Serial.println(JSON.stringify(root));
}
