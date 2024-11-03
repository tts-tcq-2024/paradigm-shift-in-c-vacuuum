#include <stdio.h>
#include "DisplayOutput.h"

const char* GetNormalBreachMessage(BreachType_en breachType) {
  switch (breachType) {
    case TOO_LOW: return "TOO LOW";
    case TOO_HIGH: return "TOO HIGH";
    default: return "NORMAL";
  }
}

const char* GetWarningMessage(BreachType_en breachType, BatteryParameter param) {
  switch (breachType) {
    case APPROACHING_LOW: return param.lowWarningMessage;
    case APPROACHING_HIGH: return param.highWarningMessage;
    default: return "NORMAL";
  }
}

const char* AssignBreachMessage(BreachType_en breachType, BatteryParameter param) {
  if (breachType == TOO_LOW || breachType == TOO_HIGH) {
    return GetNormalBreachMessage(breachType);
  } else {
    return GetWarningMessage(breachType, param);
  }
}

void DisplayOutput(BreachType_en breachType, BatteryStatusType_en status, BatteryParameter param, void (*outputFunc)(const char*)) {
  char message[100];
  snprintf(message, sizeof(message), "%s: %s", 
           (status == TEMP_OUT_OF_RANGE ? "Temperature is out of range" : 
           (status == SOC_OUT_OF_RANGE ? "State of Charge is out of range" : "Charge Rate is out of range")),
           AssignBreachMessage(breachType, param));
  outputFunc(message);
}
