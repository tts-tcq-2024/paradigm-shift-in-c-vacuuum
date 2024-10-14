#include <stdio.h>
#include <assert.h>
#include "checker.h"

BatteryCheckResult_st checkLimits(float value, BatteryParameter param) {
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

int checkParameterLowerLimitBreachAndTolerance(float value, BatteryParameter param) {
  if(value >= param.lowerLimit && value < param.lowerLimit + param.warningTolerance) {
    return 1;
  }
  return 0;
}

int checkParameterUpperLimitBreachAndTolerance(float value, BatteryParameter param) {
  if(value <= param.upperLimit && value > param.upperLimit - param.warningTolerance) {
    return 1;
  }
  return 0;
}

BatteryCheckResult_st checkTolerance(float value, BatteryParameter param) {
  BatteryCheckResult_st result = {BATTERY_OK, NORMAL};
  
  if (checkParameterLowerLimitBreachAndTolerance(value,param)) {
    result.breachType = APPROACHING_LOW;
  } else if (checkParameterUpperLimitBreachAndTolerance(value,param)) {
    result.breachType = APPROACHING_HIGH;
  }
  return result;
}

BatteryCheckResult_st checkParameter(float value, BatteryParameter param) {
  BatteryCheckResult_st result = checkLimits(value, param);

  if (result.breachType == NORMAL) {
    result = checkTolerance(value, param);
  }

  return result;
}

BatteryCheckResult_st checkTemperature(float temperature, BatteryParameter param) {
  param.lowWarningMessage = "Approaching low temperature limit";
  param.highWarningMessage = "Approaching high temperature limit";
  return checkParameter(temperature, param);
}

BatteryCheckResult_st checkSoc(float soc, BatteryParameter param) {
  param.lowWarningMessage = "Approaching discharge";
  param.highWarningMessage = "Approaching charge-peak";
  return checkParameter(soc, param);
}

BatteryCheckResult_st checkChargeRate(float chargeRate, BatteryParameter param) {
  param.lowWarningMessage = "Approaching low charge rate";
  param.highWarningMessage = "Approaching high charge rate";
  return checkParameter(chargeRate, param);
}

const char* getNormalBreachMessage(BreachType_en breachType) {
  switch (breachType) {
    case TOO_LOW: return "TOO LOW";
    case TOO_HIGH: return "TOO HIGH";
    default: return "NORMAL";
  }
}

const char* getWarningMessage(BreachType_en breachType, BatteryParameter param) {
  switch (breachType) {
    case APPROACHING_LOW: return param.lowWarningMessage;
    case APPROACHING_HIGH: return param.highWarningMessage;
    default: return "NORMAL";
  }
}

const char* assignBreachMessage(BreachType_en breachType, BatteryParameter param) {
  if (breachType == TOO_LOW || breachType == TOO_HIGH) {
    return getNormalBreachMessage(breachType);
  } else {
    return getWarningMessage(breachType, param);
  }
}

const char* assignStatusMessage(BatteryStatusType_en status) {
  if(status == TEMP_OUT_OF_RANGE) {
    return "Temperature is out of range";
  } else if(status == SOC_OUT_OF_RANGE) {
    return "State of Charge is out of range";
  } else {
    return "Charge Rate is out of range";
  }
}

void DisplayOutput(BreachType_en breachType, BatteryStatusType_en status, BatteryParameter param) {
  printf("%s: %s\n", assignStatusMessage(status), assignBreachMessage(breachType, param));
}

void DisplayMultipleBreaches(BatteryCheckResult_st* breaches, BatteryParameter* params, int breachCount) {
  for (int index = 0; index < breachCount; index++) {
    DisplayOutput(breaches[index].breachType, breaches[index].status, params[index]);
  }
}

void checkForBreach(BatteryCheckResult_st* breaches, BatteryParameter* params, int breachCount) {
  if (breachCount > 0) {
    DisplayMultipleBreaches(breaches, params, breachCount);
  }
}

int checkIfBatteryIsOk(BatteryCheckResult_st result) {
  if (result.status != BATTERY_OK || result.breachType != NORMAL) {
    return 1;
  }
  return 0;
}

int batteryIsOk(float temperature, float soc, float chargeRate) {
  int ret = 0;

  BatteryParameter tempParam = {0, 45, TEMP_WARNING_TOLERANCE};
  BatteryParameter socParam = {20, 80, SOC_WARNING_TOLERANCE};
  BatteryParameter chargeParam = {0.5, 0.8, CHARGE_RATE_WARNING_TOLERANCE};

  CheckStrategy strategies[] = {
    {checkTemperature}, {checkSoc}, {checkChargeRate}
  };

  float values[] = {temperature, soc, chargeRate};
  BatteryParameter params[] = {tempParam, socParam, chargeParam};

  BatteryCheckResult_st breaches[3];
  int breachCount = 0;
  
  for (int index = 0; index < 3; index++) {
    BatteryCheckResult_st result = strategies[index].check(values[index], params[index]);
    if (checkIfBatteryIsOk(result)) {
      breaches[breachCount++] = result;
      ret = 1;
    }
  }
  checkForBreach(breaches, params, breachCount);
  return ret;
}

int main() {
  assert(batteryIsOk(-30, 70, 0.7) == 1);
  assert(batteryIsOk(40, 10, 0.6) == 1);
  assert(batteryIsOk(25, 70, 0.9) == 1);
  assert(batteryIsOk(30, 50, 0.55) == 0);
  assert(batteryIsOk(-10, 90, 0.7) == 1);
  assert(batteryIsOk(45, 100, 0.8) == 1);

  return 0;
}
