#include <Arduino.h>

#include "scheduler.h"
#include "task2.h"

#define LEDPIN 12
#define OFF HIGH
#define ON LOW

void task2Func(void *arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
#ifdef DOTRACE
    Serial.println("In task2");
#endif
    mutex1->lock();
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
    mutex1->unlock();

    Sched_Yield(); // yield
  }
}
