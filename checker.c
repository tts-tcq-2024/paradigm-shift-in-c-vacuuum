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


BatterySts_en batteryIsOk(float temperature, float soc, float chargeRate) {

  BatterySts_en ret = BATTERY_OK;

  if ((temperature < 0 || temperature > 45) || 
  (soc < 20 || soc > 80) || 
  (chargeRate > 0.8)) 
  {
    if (temperature < 0 || temperature > 45)
    {
      ret = TEMP_OUT_OF_RANGE;
    }
    else if (soc < 20 || soc > 80)
    {
      ret = SOC_OUT_OF_RANGE;
    }
    else
    {
    ret = CHARGE_RATE_OUT_OF_RANGE;
    }
  }
  return ret;
}

int main() 
{
  assert(batteryIsOk(-30, 70, 0.7) == 1);
  assert(batteryIsOk(40, 10, 0) == 2);
  assert(batteryIsOk(25, 70, 0.9) == 3);
  assert(batteryIsOk(20, 50, 0.5) == 0);
}
