#include <Arduino.h>

#include "include/scheduler.h"
#include "include/task4.h"

#define LEDPIN 10
#define OFF HIGH
#define ON LOW

void task4Func(void *arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
#ifdef DEBUG
    Serial.println("In task4");
#endif
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield(); // yield
  }
}
