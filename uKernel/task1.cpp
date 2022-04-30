#include <Arduino.h>

#define LEDPIN 13

void task1Func(void* arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  while (true) {
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
    delay(1000);
  }
}
