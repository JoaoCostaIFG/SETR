#include <Arduino.h>

#include "include/task3.h"

#define LEDPIN 11

void task3Func(void* arg) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
