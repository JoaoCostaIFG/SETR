#include <Arduino.h>

#include "task2.h"

#define LEDPIN 12

void Task2::init() {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
}

void Task2::run() {
  digitalWrite(LEDPIN, HIGH);
  delay(500);
  digitalWrite(LEDPIN, LOW);
}
