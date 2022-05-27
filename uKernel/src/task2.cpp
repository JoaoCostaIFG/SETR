#include <Arduino.h>

#include "sched/scheduler.h"
#include "task2.h"

#define LEDPIN 12
#define BUTTON A1
#define OFF LOW
#define ON HIGH

void task2Func(void *arg) {
  // set pin as output
  while (true) {
#ifdef DOTRACE
    //Serial.println("\nIn task2");
#endif
    Sched_Yield(); // yield
  }
}
