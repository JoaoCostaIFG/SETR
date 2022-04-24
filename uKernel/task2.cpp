#include <Arduino.h>

#include "task2.h"

#define LEDPIN 12

void initTask2() {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
}

void runTask2() {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
