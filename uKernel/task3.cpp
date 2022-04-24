#include <Arduino.h>

#include "task3.h"

#define LEDPIN 11

void initTask3() {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
}

void runTask3() {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
