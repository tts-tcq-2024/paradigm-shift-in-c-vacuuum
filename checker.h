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

typedef struct {
  BatteryCheckResult_st (*check)(float, BatteryParameter);
} CheckStrategy;
