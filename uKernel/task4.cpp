#include <Arduino.h>

#include "include/task4.h"
#include "include/scheduler.h"

#define LEDPIN 10

void task4Func(void* arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  while (true) {
    Serial.println("In task4");
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield();
  }
}
