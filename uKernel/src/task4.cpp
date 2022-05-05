#include <Arduino.h>

#include "scheduler.h"
#include "task4.h"

#define LEDPIN 10
#define OFF LOW
#define ON HIGH

void task4Func(void *arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
#ifdef DOTRACE
    Serial.println("In task4");
#endif
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield(); // yield
  }
}
