#include <Arduino.h>

#include "include/task1.h"

#define LEDPIN 13

void Task1::init() {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
}

void Task1::run() {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
  delay(700);
}
