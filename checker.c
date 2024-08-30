#include <stdio.h>
#include <assert.h>
#include <string.h>

// enumeration to avoid multiple print statements
typedef enum 
{
  BATTERY_OK,
  TEMP_OUT_OF_RANGE,
  SOC_OUT_OF_RANGE,
  CHARGE_RATE_OUT_OF_RANGE
} BatterySts_en;

// Separate functions for each condition

int isTempOrSocOutOfRange(float qty, float ll, float ul) 
{
  return (qty < ll || qty > ul);
}

int isChargeRateOutOfRange(float chargeRate)
{
  return (chargeRate > 0.8);
}

BatterySts_en checkTemperature(float temperature)
{
  if (isTempOrSocOutOfRange(temperature, 0, 45))
  {
    return TEMP_OUT_OF_RANGE;
  }
  return BATTERY_OK;
}

BatterySts_en checkSoc(float soc)
{
  if (isTempOrSocOutOfRange(soc, 20, 80))
  {
    return SOC_OUT_OF_RANGE;
  }
  return BATTERY_OK;
}

BatterySts_en checkChargeRate(float chargeRate)
{
  if (isChargeRateOutOfRange(chargeRate))
  {
    return CHARGE_RATE_OUT_OF_RANGE;
  }
  return BATTERY_OK;
}

BatterySts_en batteryIsOk(float temperature, float soc, float chargeRate)
{
  BatterySts_en status;

  status = checkTemperature(temperature);
  if (status != BATTERY_OK) 
  {
    return status;
  }

  status = checkSoc(soc);
  if (status != BATTERY_OK) 
  {
    return status;
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

