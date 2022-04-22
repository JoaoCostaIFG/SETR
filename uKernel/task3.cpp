#include <Arduino.h>

#include "task3.h"

#define LEDPIN 11

void Task3::init(void) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
}

void Task3::run(void) {
  digitalWrite(LEDPIN, HIGH);
  delay(100);
  digitalWrite(LEDPIN, LOW);
}
