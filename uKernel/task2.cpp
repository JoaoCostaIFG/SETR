#include <Arduino.h>

#define LEDPIN 12

void task2Func(void* arg) {
  //set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
