#include <Arduino.h>

#include "task1.h"

#define LEDPIN 13

void Task1::init(void) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
}

void Task1::run(void) {
  digitalWrite(LEDPIN, HIGH);
  delay(1000);
  digitalWrite(LEDPIN, LOW);
}
