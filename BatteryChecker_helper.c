#include "BatteryChecker.h"

int CheckParameterLowerLimitBreachAndTolerance(float value, BatteryParameter param) {
  if(value >= param.lowerLimit && value < param.lowerLimit + param.warningTolerance) {
    return 1;
  }
  return 0;
}

int CheckParameterUpperLimitBreachAndTolerance(float value, BatteryParameter param) {
  if(value <= param.upperLimit && value > param.upperLimit - param.warningTolerance) {
    return 1;
  }
  return 0;
}

int CheckIfBatteryIsOk(BatteryCheckResult_st result) {
  if (result.status != BATTERY_OK || result.breachType != NORMAL) {
    return 1;
  }
  return 0;
}