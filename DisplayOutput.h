#ifndef DISPLAYOUTPUT_H
#define DISPLAYOUTPUT_H

#include "BatteryChecker.h"

const char* GetNormalBreachMessage(BreachType_en breachType);
const char* GetWarningMessage(BreachType_en breachType, BatteryParameter param);
const char* AssignBreachMessage(BreachType_en breachType, BatteryParameter param);
void DisplayOutput(BreachType_en breachType, BatteryStatusType_en status, BatteryParameter param, void (*outputFunc)(const char*));

#endif // DISPLAYOUTPUT_H
