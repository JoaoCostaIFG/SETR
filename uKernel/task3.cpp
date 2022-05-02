#include <Arduino.h>

#include "include/task3.h"
#include "include/scheduler.h"

#define LEDPIN 11

void task3Func(void* arg) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  while (true) {
    Serial.println("In task3");
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield();
  }
}
