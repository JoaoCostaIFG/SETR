#ifdef RUNTIMEASSERT

#include <util/delay.h>

#include "sched/assert.h"
#include "sched/scheduler.h"

#define ON HIGH
#define OFF LOW
#define SHORT_BLINK 300
#define LONG_BLINK (SHORT_BLINK * 3)

void assertFailed(const __FlashStringHelper *const file, const int line,
                  const char *const func,
                  const __FlashStringHelper *const msg) {
  // stop scheduler (kernel ticks) so no more tasks run (preemption)
  Sched_Stop();

  // communicate the error
  Serial.print(file);
  Serial.print(F(":"));
  //Serial.print(line); // this sometimes causes a gcc bug
  Serial.print(F(" "));
  Serial.print(func);
  Serial.println(F("():"));
  Serial.print(F("Assert failed: "));
  Serial.println(msg);

  // blink SOS in morse-code ad infinitum
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

#endif // RUNTIMEASSERT
