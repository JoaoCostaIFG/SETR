#include <Arduino.h>

#include "include/assert.h"

#define ON HIGH
#define OFF LOW
#define SHORT_BLINK 300
#define LONG_BLINK SHORT_BLINK * 3

void assertFailed() {
  noInterrupts();

  pinMode(LED_BUILTIN, OUTPUT);

  while (true) {
    // 3 short blinks
    for (int i = 0; i < 3; ++i) {
      digitalWrite(LED_BUILTIN, ON);
      _delay_ms(SHORT_BLINK);
      digitalWrite(LED_BUILTIN, OFF);
      if (i + 1 < 3)
        _delay_ms(SHORT_BLINK);
    }

    _delay_ms(SHORT_BLINK);

    // 3 long blinks
    for (int i = 0; i < 3; ++i) {
      digitalWrite(LED_BUILTIN, ON);
      _delay_ms(LONG_BLINK);
      digitalWrite(LED_BUILTIN, OFF);
      if (i + 1 < 3)
        _delay_ms(LONG_BLINK);
    }

    _delay_ms(SHORT_BLINK);

    // 3 short blinks
    for (int i = 0; i < 3; ++i) {
      digitalWrite(LED_BUILTIN, ON);
      _delay_ms(SHORT_BLINK);
      digitalWrite(LED_BUILTIN, OFF);
      if (i + 1 < 3)
        _delay_ms(SHORT_BLINK);
    }

    // long pause
    _delay_ms(LONG_BLINK * 2);
  }
}
