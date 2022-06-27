#include <Arduino.h>

#include "sched/scheduler.h"
#include "task2.h"

#define LEDPIN 12
#define OFF LOW
#define ON HIGH

void task2Func(void *arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, OFF);


  while (true) {
    Serial.println("In task2");

    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield(); // yield
  }
}
