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

  return (temperature < 0 || temperature > 45) ? TEMP_OUT_OF_RANGE :
          (soc < 20 || soc > 80) ? SOC_OUT_OF_RANGE :
          (chargeRate > 0.8) ? CHARGE_RATE_OUT_OF_RANGE : BATTERY_OK;
}

int main() 
{
  assert(batteryIsOk(-30, 70, 0.7) == 1);
  assert(batteryIsOk(40, 10, 0) == 2);
  assert(batteryIsOk(25, 70, 0.9) == 3);
  assert(batteryIsOk(20, 50, 0.5) == 0);
}

