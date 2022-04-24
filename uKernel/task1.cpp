#include <Arduino.h>

#include "task1.h"

#define LEDPIN 13

void initTask1() {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
}

void runTask1() {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
  //delay(1000);
}
