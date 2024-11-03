#include "BatteryChecker.h"
#include "DisplayOutput.h"

BatteryCheckResult_st CheckLimits(float value, BatteryParameter param) {
  BatteryCheckResult_st result = {BATTERY_OK, NORMAL};
  if (value < param.lowerLimit) {
    result.status = SOC_OUT_OF_RANGE;
    result.breachType = TOO_LOW;
  } else if (value > param.upperLimit) {
    result.status = SOC_OUT_OF_RANGE;
    result.breachType = TOO_HIGH;
  }
  return result;
}

BatteryCheckResult_st CheckTolerance(float value, BatteryParameter param) {
  BatteryCheckResult_st result = {BATTERY_OK, NORMAL};
  if (CheckParameterLowerLimitBreachAndTolerance(value,param)) {
    result.breachType = APPROACHING_LOW;
  } else if (CheckParameterUpperLimitBreachAndTolerance(value,param)) {
    result.breachType = APPROACHING_HIGH;
  }
  return result;
}

BatteryCheckResult_st CheckParameter(float value, BatteryParameter param) {
  BatteryCheckResult_st result = CheckLimits(value, param);
  if (result.breachType == NORMAL) {
    result = CheckTolerance(value, param);
  }
  return result;
}

int BatteryIsOk(float temperature, float soc, float chargeRate, void (*outputFunc)(const char*)) {
  int isBatteryOk = 1;
  BatteryParameter params[] = {
    {0, 45, TEMP_WARNING_TOLERANCE, "Approaching low temperature limit", "Approaching high temperature limit"},
    {20, 80, SOC_WARNING_TOLERANCE, "Approaching discharge", "Approaching charge-peak"},
    {0.5, 0.8, CHARGE_RATE_WARNING_TOLERANCE, "Approaching low charge rate", "Approaching high charge rate"}};
  float values[] = {temperature, soc, chargeRate};
  BatteryStatusType_en statusTypes[] = {TEMP_OUT_OF_RANGE, SOC_OUT_OF_RANGE, CHARGE_RATE_OUT_OF_RANGE};
  for (int i = 0; i < 3; i++) {
    BatteryCheckResult_st result = CheckParameter(values[i], params[i]);
    if (CheckIfBatteryIsOk(result)) {
      isBatteryOk = 0;
      DisplayOutput(result.breachType, statusTypes[i], params[i], outputFunc);
    }
  }
  return isBatteryOk;
}
