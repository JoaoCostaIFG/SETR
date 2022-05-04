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
void assertFailed();

/** If the given test fails, call assertFailed */
#define assert(test) if (!(test)) assertFailed()

#else

#define assert(test)

#endif

#endif // ASSERT_H
