#include <Arduino.h>

#include "task2.h"

#define LEDPIN 12

void Task2::init(void) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
}

void Task2::run(void) {
  digitalWrite(LEDPIN, HIGH);
  delay(500);
  digitalWrite(LEDPIN, LOW);
}
