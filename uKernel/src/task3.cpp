#include <Arduino.h>

#include "sched/scheduler.h"
#include "task3.h"

#define LEDPIN 11
#define BUTTON A1
#define OFF HIGH
#define ON LOW

void task3Func(void *arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, OFF);
  pinMode(BUTTON, INPUT);

  while (true) {
    Serial.println("In task3");

    mutex1->lock();

    digitalWrite(LEDPIN, ON);
    while (digitalRead(BUTTON)) {}
    digitalWrite(LEDPIN, OFF);

    mutex1->unlock();

    Sched_Yield(); // yield
  }
}
