#include <Arduino.h>

#include "task1.h"

#define LEDPIN 13

void initTask1(void) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
}

void task1(void) {
  digitalWrite(LEDPIN, HIGH);
  delay(1000);
  digitalWrite(LEDPIN, LOW);
}
