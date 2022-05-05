#include <Arduino.h>

#include "scheduler.h"
#include "task3.h"

#define LEDPIN 11
#define OFF LOW
#define ON HIGH

void task3Func(void *arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
#ifdef DOTRACE
    Serial.println("In task3");
#endif
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield(); // yield
  }
}
