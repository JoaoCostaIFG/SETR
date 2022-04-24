#include <Arduino.h>

#include "include/task4.h"

#define LEDPIN 10

void Task4::init() {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
}

void Task4::run() {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
