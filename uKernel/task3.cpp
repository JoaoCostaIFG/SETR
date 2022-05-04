#include <Arduino.h>

#include "include/task3.h"
#include "include/scheduler.h"

#define LEDPIN 11
#define OFF     HIGH
#define ON      LOW

void task3Func(void* arg) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
#ifdef DEBUG
    Serial.println("In task3");
#endif
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield(); // yield
  }
}
