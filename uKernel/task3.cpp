#include <Arduino.h>

#include "task3.h"

#define LEDPIN 11

void initTask3(void) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
}

void task3(void) {
  digitalWrite(LEDPIN, HIGH);
  delay(100);
  digitalWrite(LEDPIN, LOW);
}
