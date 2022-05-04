#include <Arduino.h>

#include "include/task1.h"
#include "include/scheduler.h"

#define LEDPIN  13
#define OFF     HIGH
#define ON      LOW

void task1Func(void* arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
#ifdef DEBUG
    Serial.println("In task1");
#endif
    mutex1->lock();
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
    mutex1->unlock();

    Sched_Yield(); // yield
  }
}
