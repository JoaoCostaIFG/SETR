#include <Arduino.h>

#include "include/task2.h"
#include "include/scheduler.h"

#define LEDPIN 12
#define OFF     HIGH
#define ON      LOW

void task2Func(void* arg) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
#ifdef DEBUG
    Serial.println("In task2");
#endif
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield(); // yield
  }
}
