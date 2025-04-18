#include <Arduino.h>
#include <Arduino_JSON.h>
#include "network_info.h"
#include "globals.h"

void printStatus() {
    NetworkInfo info = computeNetworkInfo(
        ipConfigs[currentIpIndex].ipByte,
        ipConfigs[currentIpIndex].subnetByte
    );

    JSONVar root;
    root["subnet"] = info.subnet;

    JSONVar ipArray;
    JSONVar subnetArray;
    for (int i = 0; i < 4; i++) {
        ipArray[i] = ipConfigs[i].ipByte;
        subnetArray[i] = ipConfigs[i].subnetByte;
    }
    root["ip"] = ipArray;
    root["subnets"] = subnetArray;
    root["current"] = currentIpIndex;


    JSONVar usable;
    usable[0] = info.usableMin;
    usable[1] = info.usableMax;
    root["usable"] = usable;

    root["hosts"] = info.totalHosts;
    root["cidr"] = info.cidrSuffix + 24;

    Serial.println(JSON.stringify(root));
}
