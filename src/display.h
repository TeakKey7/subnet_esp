#pragma once

#include <Adafruit_SSD1306.h>
#include "network_info.h"

// Assume display is declared in main.ino
extern Adafruit_SSD1306 display;
extern byte currentIpIndex;

inline void updateDisplay() {
  byte ip = ipConfigs[currentIpIndex].ipByte;
  byte subnet = ipConfigs[currentIpIndex].subnetByte;

  NetworkInfo info = computeNetworkInfo(ip, subnet);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.print("IP #");
  display.print(currentIpIndex);
  display.print(": 192.168.1.");
  display.println(info.ip);

  display.print("CIDR: /");
  display.println(info.cidrSuffix + 24);

  display.print("Subnet: ");
  display.println(info.subnet);

  display.print("Range: ");
  display.print("192.168.1.");
  display.print(info.usableMin);
  display.print("-");
  display.println(info.usableMax);

  display.print("Hosts: ");
  display.println(info.totalHosts);

  display.display();
}
