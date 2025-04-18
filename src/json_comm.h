#pragma once

#include <Arduino.h>
#include <Arduino_JSON.h>
#include "globals.h"
#include "util.h"

// 🔧 Forward declaration so it's known to readCommand()
inline void processCommand(const String& json);

inline void readCommand() {
  static String inputBuffer;

  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      processCommand(inputBuffer);
      inputBuffer = "";
    }
    else {
      inputBuffer += c;
    }
  }
}

inline void processCommand(const String& json) {
  JSONVar doc = JSON.parse(json);
  if (JSON.typeof(doc) != "object") return;

  // Update single IP value
  if (doc.hasOwnProperty("setIp")) {
    int ipVal = (int)doc["setIp"];
    if (ipVal >= 0 && ipVal <= 255) {
      ipConfigs[currentIpIndex].ipByte = (byte)ipVal;
    }
  }

  // Update current index
  if (doc.hasOwnProperty("setIndex")) {
    int idx = (int)doc["setIndex"];
    if (idx >= 0 && idx < 4) {
      currentIpIndex = idx;
    }
  }

  // Update subnet for current index
  if (doc.hasOwnProperty("setSubnet")) {
    int raw = (int)doc["setSubnet"];
    if (raw >= 0 && raw <= 255 && isValidSubnet((byte)raw)) {
      ipConfigs[currentIpIndex].subnetByte = (byte)raw;
    }
  }

  // Update subnet for specific index
  if (doc.hasOwnProperty("setSubnetAt")) {
    JSONVar sub = doc["setSubnetAt"];
    if (sub.hasOwnProperty("index") && sub.hasOwnProperty("value")) {
      int idx = (int)sub["index"];
      int val = (int)sub["value"];
      if (idx >= 0 && idx < 4 && val >= 0 && val <= 255 && isValidSubnet((byte)val)) {
        ipConfigs[idx].subnetByte = (byte)val;
      }
    }
  }
}
