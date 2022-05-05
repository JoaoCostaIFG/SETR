#ifndef ASSERT_H
#define ASSERT_H

#ifdef RUNTIMEASSERT

/**
 * Blocks the device in a loop saying SOS in morse:
 * 3 short blink
 * 3 long  blinks
 * 3 short blinks
 * (repeat)
 */
void assertFailed(const __FlashStringHelper *file, int line, const char *func,
                  const __FlashStringHelper *msg);

/** If the given test fails, call assertFailed */
#define assertCond(test, msg)                                                  \
  if (!(test))                                                                 \
  assertFailed(F(__FILE__), __LINE__, __func__, msg)

#else

#define assertCond(test, msg)

#endif

#endif // ASSERT_H
