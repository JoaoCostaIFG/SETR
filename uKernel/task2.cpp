#include <Arduino.h>

#include "task2.h"

#define LEDPIN 12

void initTask2(void) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
}

void task2(void) {
  digitalWrite(LEDPIN, HIGH);
  delay(500);
  digitalWrite(LEDPIN, LOW);
}
