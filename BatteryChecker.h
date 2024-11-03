#ifndef BATTERYCHECKER_H
#define BATTERYCHECKER_H

#include <stdio.h>

#define SOC_WARNING_TOLERANCE 4
#define TEMP_WARNING_TOLERANCE 2.25
#define CHARGE_RATE_WARNING_TOLERANCE 0.04

typedef enum {
  BATTERY_OK,
  TEMP_OUT_OF_RANGE,
  SOC_OUT_OF_RANGE,
  CHARGE_RATE_OUT_OF_RANGE
} BatteryStatusType_en;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH,
  APPROACHING_LOW,
  APPROACHING_HIGH
} BreachType_en;

typedef struct {
  BatteryStatusType_en status;
  BreachType_en breachType;
} BatteryCheckResult_st;

typedef struct {
  float lowerLimit;
  float upperLimit;
  float warningTolerance;
  const char* lowWarningMessage;
  const char* highWarningMessage;
} BatteryParameter;

typedef BatteryCheckResult_st (*CheckFunc)(float, BatteryParameter);
int BatteryIsOk(float temperature, float soc, float chargeRate, void (*outputFunc)(const char*));
BatteryCheckResult_st CheckParameter(float value, BatteryParameter param);
BatteryCheckResult_st CheckTolerance(float value, BatteryParameter param);
BatteryCheckResult_st CheckLimits(float value, BatteryParameter param);
int CheckParameterLowerLimitBreachAndTolerance(float value, BatteryParameter param);
int CheckParameterUpperLimitBreachAndTolerance(float value, BatteryParameter param);
int CheckIfBatteryIsOk(BatteryCheckResult_st result);
#endif // BATTERYCHECKER_H
