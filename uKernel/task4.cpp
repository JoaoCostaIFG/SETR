#include <Arduino.h>

#include "include/task4.h"
#include "include/scheduler.h"

#define LEDPIN 10

void task4Func(void* arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  while (true) {
#ifdef DEBUG
    Serial.println("In task4");
#endif
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield();
  }
}
