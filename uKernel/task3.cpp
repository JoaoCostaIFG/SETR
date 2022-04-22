#include <Arduino.h>

#include "task3.h"

#define LEDPIN 11

void Task3::init() {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
}

void Task3::run() {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
