#include <assert.h>
#include <stdio.h>
#include "BatteryChecker.h"

void ConsoleOutput(const char* message) {
  printf("%s\n", message);
}

int main() {
  assert(BatteryIsOk(-30, 70, 0.7, ConsoleOutput) == 0);
  assert(BatteryIsOk(40, 10, 0.6, ConsoleOutput) == 0);
  assert(BatteryIsOk(25, 70, 0.9, ConsoleOutput) == 0);
  assert(BatteryIsOk(30, 50, 0.55, ConsoleOutput) == 1);
  assert(BatteryIsOk(-10, 90, 0.7, ConsoleOutput) == 0);
  assert(BatteryIsOk(45, 100, 0.8, ConsoleOutput) == 0);
  return 0;
}
