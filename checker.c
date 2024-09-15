#include <stdio.h>
#include <assert.h>

typedef enum {
  BATTERY_OK,
  TEMP_OUT_OF_RANGE,
  SOC_OUT_OF_RANGE,
  CHARGE_RATE_OUT_OF_RANGE
} BatteryStatusType_en;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType_en;

typedef struct {
  BatteryStatusType_en status;
  BreachType_en breachType;
} BatteryCheckResult_st;

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

BatteryCheckResult_st checkChargeRate(float chargeRate)
{
  BatteryCheckResult_st result = {BATTERY_OK, NORMAL};
  if (chargeRate < 0.5)
  {
    result.status = SOC_OUT_OF_RANGE;
    result.breachType = TOO_LOW;
  }
  else if (chargeRate > 0.8)
  {
    result.status = CHARGE_RATE_OUT_OF_RANGE;
    result.breachType = TOO_HIGH;
  }
  return result;
}

const char* assignStatusMessage(BatteryStatusType_en status)
{
  const char *statusMessage;
  if(status == TEMP_OUT_OF_RANGE)
  {
    statusMessage = "Temperature is out of range";
  }
  else if(status == SOC_OUT_OF_RANGE)
  {
    statusMessage = "State of Charge is out of range";
  }
  statusMessage = "Charge Rate is out of range";
  return statusMessage;
}

const char* assignBreachMessage(BreachType_en breachType)
{
  const char *breachMessage;

  if(breachType == TOO_LOW)
  {
    breachMessage = "TOO LOW";
  }
  else if(breachType == TOO_HIGH)
  {
    breachMessage = "TOO HIGH";
  }
  return breachMessage;

}

void DisplayOutput(BreachType_en breachType, BatteryStatusType_en status)
{
  printf("%s: %s\n", assignStatusMessage(status), assignBreachMessage(breachType));
}

void DisplayMultipleBreaches(BatteryCheckResult_st* breaches, int breachCount)
{
  for (int i = 0; i < breachCount; i++)
  {
    DisplayOutput(breaches[i].breachType, breaches[i].status);
  }
}

void checkForBreach(BatteryCheckResult_st* breaches, int breachCount)
{
  if (breachCount > 0)
  {
    DisplayMultipleBreaches(breaches, breachCount);
  }
}

typedef BatteryCheckResult_st (*CheckFunction)(float);

int batteryIsOk(float temperature, float soc, float chargeRate)
{
  int ret = 0;
  CheckFunction checkFunctions[] = {checkTemperature, checkSoc, checkChargeRate};
  float values[] = {temperature, soc, chargeRate};
  BatteryCheckResult_st breaches[3];
  int breachCount = 0;
  for (int i = 0; i < 3; i++)
  {
    BatteryCheckResult_st result = checkFunctions[i](values[i]);
    if (result.status != BATTERY_OK)
    {
      breaches[breachCount++] = result;
      ret = 1;
    }
  }
  checkForBreach(breaches, breachCount);
  printf("\n");
  return ret;
}

int main() 
{
  assert(batteryIsOk(-30, 70, 0.7) == 1);
  assert(batteryIsOk(40, 10, 0.6) == 1);
  assert(batteryIsOk(25, 70, 0.9) == 1);
  assert(batteryIsOk(20, 50, 0.5) == 0);
  // Multiple quantities out of range
  assert(batteryIsOk(-10, 90, 0.7) == 1);
  assert(batteryIsOk(45, 100, 0.8) == 1);
  assert(batteryIsOk(99.98,55.8,0.801) == 1);
  assert(batteryIsOk(-0.1,19.99,0.256) == 1);
  assert(batteryIsOk(59.90,97.647,0.82) == 1);
  return 0;
}
