#include <Arduino.h>

#include "sched/scheduler.h"
#include "task1.h"

#define LEDPIN 13
#define OFF HIGH
#define ON LOW

void task1Func(void *arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, OFF);

  while (true) {
    Serial.println("In task1");
    digitalWrite(LEDPIN, ON);

    mutex1->lock();
    digitalWrite(LEDPIN, OFF);
    mutex1->unlock();

    Sched_Yield(); // yield
  }
}
