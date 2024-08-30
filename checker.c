#include <stdio.h>
#include <assert.h>
#include <string.h>

// enumeration to avoid multiple print statements
typedef enum {
  BATTERY_OK,
  TEMP_OUT_OF_RANGE,
  SOC_OUT_OF_RANGE,
  CHARGE_RATE_OUT_OF_RANGE
} BatterySts_en;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType_en;

typedef struct {
  BatterySts_en status;
  BreachType_en breachType;
} BatteryCheckResult_st;

// Separate functions for each condition i.e helper functions
int isTempOrSocOutOfRange(float qty, float ll, float ul) 
{
  return (qty < ll || qty > ul);
}

int isChargeRateOutOfRange(float chargeRate)
{
  return (chargeRate > 0.8);
}

BatteryCheckResult_st checkTemperature(float temperature)
{
    BatteryCheckResult_st result = {BATTERY_OK, NORMAL};

    if (temperature < 0)
    {
        result.status = TEMP_OUT_OF_RANGE;
        result.breachType = TOO_LOW;
    }
    else if (temperature > 45)
    {
        result.status = TEMP_OUT_OF_RANGE;
        result.breachType = TOO_HIGH;
    }

    return result;
}

BatteryCheckResult_st checkSoc(float soc)
{
  BatteryCheckResult_st result = {BATTERY_OK, NORMAL};

  if (soc < 20)
  {
    result.status = SOC_OUT_OF_RANGE;
    result.breachType = TOO_LOW;
  }
  else if (soc > 80)
  {
    result.status = SOC_OUT_OF_RANGE;
    result.breachType = TOO_HIGH;
  }

  return result;
}

BatterySts_en checkChargeRate(float chargeRate)
{
  BatteryCheckResult_st result = {BATTERY_OK, NORMAL};

  if (chargeRate > 0.8)
  {
    result.status = CHARGE_RATE_OUT_OF_RANGE;
    result.breachType = TOO_HIGH;
  }

  return result.status;
}

BatterySts_en batteryIsOk(float temperature, float soc, float chargeRate)
{
  BatteryCheckResult_st result;

  result = checkTemperature(temperature);
  if (result.status != BATTERY_OK)
  {
    return result.status;
  }

  result = checkSoc(soc);
  if (result.status != BATTERY_OK)
  {
    return result.status;
  }

  return checkChargeRate(chargeRate);
}

int main() 
{
  assert(batteryIsOk(-30, 70, 0.7) == 1);
  assert(batteryIsOk(40, 10, 0) == 2);
  assert(batteryIsOk(25, 70, 0.9) == 3);
  assert(batteryIsOk(20, 50, 0.5) == 0);
}
