#include <Arduino.h>

#define LEDPIN  13
#define OFF     HIGH
#define ON      LOW

void task1Func(void* arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, ON);

  while (true) {
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
    delay(1000);
  }
}
