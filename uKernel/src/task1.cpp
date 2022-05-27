#include <Arduino.h>
#include <string.h>

#include "sched/scheduler.h"
#include "task1.h"

#define LEDPIN 13
#define OFF LOW
#define ON HIGH

void task1Func(void* arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
#ifdef DOTRACE
    Serial.println("In task1");
#endif

    volatile unsigned int a = 0;

    for (unsigned int i = 0; i < 30000; ++i) {
      for (unsigned int j = 0; j < 30000; ++j) {
        a += strlen("asdasdasdasdqwea");
      }
    }

#ifdef DOTRACE
    Serial.println("Out task1");
#endif

    Sched_Yield(); // yield
  }
}
