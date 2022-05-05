#include <Arduino.h>

#include "scheduler.h"
#include "task1.h"

#define LEDPIN 13
#define OFF LOW
#define ON HIGH

void task1Func(void *arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
#ifdef DOTRACE
    Serial.println("In task1");
#endif
    mutex1->lock();
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
    mutex1->unlock();

    Sched_Yield(); // yield
  }
}
