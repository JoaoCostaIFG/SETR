#include <Arduino.h>

#include "include/task2.h"
#include "include/scheduler.h"

#define LEDPIN 12

void task2Func(void* arg) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  while (true) {
    Serial.println("In task2");
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield(); // yield
  }
}
