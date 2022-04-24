#include <Arduino.h>

#include "include/task2.h"

#define LEDPIN 12

void Task2::init() {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
}

void Task2::run() {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
