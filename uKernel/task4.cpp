#include <Arduino.h>

#include "include/task4.h"

#define LEDPIN 10

void task4Func(void* arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
