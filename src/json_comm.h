#pragma once
#include <Arduino.h>
#include <Arduino_JSON.h>
#include "globals.h"
#include "print_status.h"

inline void processCommand(const String& jsonStr) {
  JSONVar doc = JSON.parse(jsonStr);
  if (JSON.typeof(doc) != "object") return;

  if (doc.hasOwnProperty("setIp")) {
    ipConfigs[currentIpIndex].ipByte = int(doc["setIp"]);
  }

  if (doc.hasOwnProperty("setSubnet")) {
    byte index = (uint8_t)(int)doc["setSubnet"]["index"];
    byte value = (uint8_t)(int)doc["setSubnet"]["value"];
    if (index < 4) {
      ipConfigs[index].subnetByte = value;
    }
  }
  

  if (doc.hasOwnProperty("setIndex")) {
    uint8_t index = int(doc["setIndex"]);
    if (index < 4) {
      currentIpIndex = index;
      printStatus();
    }
  }
}

inline void readCommand() {
  static String inputBuffer;
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      processCommand(inputBuffer);
      inputBuffer = "";
    } else {
      inputBuffer += c;
    }
  }
}
