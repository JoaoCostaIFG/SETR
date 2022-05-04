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
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield(); // yield
  }
}
