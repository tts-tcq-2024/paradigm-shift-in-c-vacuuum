#include <stdio.h>
#include <assert.h>

#define SOC_WARNING_TOLERANCE 4 // 5% of 80
#define TEMP_WARNING_TOLERANCE 2.25  // 5% of upper limit 45
#define CHARGE_RATE_WARNING_TOLERANCE 0.04  // 5% of 0.8

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

typedef struct {
  BatteryCheckResult_st (*check)(float, BatteryParameter);
} CheckStrategy;


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
  else {
    return 0;
  }
}

int checkParameterUpperLimitBreachAndTolerance(float value, BatteryParameter param) {
  if(value <= param.upperLimit && value > param.upperLimit - param.warningTolerance) {
    return 1;
  }
  else {
    return 0;
  }
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

// Parameter check functions for each battery parameter
BatteryCheckResult_st checkTemperature(float temperature, BatteryParameter param) {
  param.lowWarningMessage = "Approaching discharge";
  param.highWarningMessage = "Approaching charge-peak";
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

const char* getWarningMessage(BreachType_en breachType) {
  switch (breachType) {
    case APPROACHING_LOW: return "Approaching low limit";
    case APPROACHING_HIGH: return "Approaching high limit";
    default: return "NORMAL";
  }
}

const char* assignBreachMessage(BreachType_en breachType) {
  if (breachType == TOO_LOW || breachType == TOO_HIGH) {
    return getNormalBreachMessage(breachType);
  } else {
    return getWarningMessage(breachType);
  }
}

const char* assignStatusMessage(BatteryStatusType_en status) {
  const char *statusMessage;
  if(status == TEMP_OUT_OF_RANGE) {
    statusMessage = "Temperature is out of range";
  } else if(status == SOC_OUT_OF_RANGE) {
    statusMessage = "State of Charge is out of range";
  } else {
    statusMessage = "Charge Rate is out of range";
  }
  return statusMessage;
}

// Display functions
void DisplayOutput(BreachType_en breachType, BatteryStatusType_en status) {
  printf("%s: %s\n", assignStatusMessage(status), assignBreachMessage(breachType));
}

void DisplayMultipleBreaches(BatteryCheckResult_st* breaches, int breachCount) {
  for (int index = 0; index < breachCount; index++) {
    DisplayOutput(breaches[index].breachType, breaches[index].status);
  }
}

void checkForBreach(BatteryCheckResult_st* breaches, int breachCount) {
  if (breachCount > 0) {
    DisplayMultipleBreaches(breaches, breachCount);
  }
}

int checkIfBatteryIsOk(BatteryCheckResult_st result)
{
  if (result.status != BATTERY_OK || result.breachType != NORMAL) {
    return 1;
  }
  else {
    return 0;
  }

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
  checkForBreach(breaches, breachCount);
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
