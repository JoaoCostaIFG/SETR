#include <Arduino.h>

#include "assert.h"
#include "scheduler.h"
#include "mutex.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

Mutex* mutex1 = new Mutex();

static Task* task1 = new Task(&task1Func, (void*) 0, 200, 2, 0 );
static Task* task2 = new Task(&task2Func, (void*) 0, 200, 20, 0, 20);
//static Task* task3 = new Task(&task3Func, (void*) 0, 128, 3, 0, 3);
//static Task* task4 = new Task(&task4Func, (void*) 0, 43, 1, 0, 1);

ISR(SCHEDULER_ISR, ISR_NAKED) __attribute__ ((hot, flatten));

ISR(SCHEDULER_ISR) {
#ifdef DOTRACE
  Serial.println("ISR");
#endif
  Sched_CtxSwitch();

  // the return from interrupt call must be explicitly added
  __asm__ __volatile__ ( "reti" );
}

void setup() {
  Sched_Init();

#if defined(DOTRACE) || defined(RUNTIMEASSERT)
  Serial.begin(9600);
  // wait for serial port to connect
  while (!Serial) {
    ;
  }
#endif

  assertCond(Sched_Add(task1) == 0, F("Failed to add task 1"));
  assertCond(Sched_Add(task2) == 0, F("Failed to add task 2"));
  //assertCond(Sched_Add(task3) == 0, F("Failed to add task 3"));

  Sched_Start();

  free(task1);
  free(task2);
  //free(task3);
  //free(task4);

  free(mutex1);
}

void loop() {
  // do nothing
}
