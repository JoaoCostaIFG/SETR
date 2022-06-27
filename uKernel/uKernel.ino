#include <Arduino.h>

#include "sched/assert.h"
#include "sched/scheduler.h"
#include "sched/Mutex.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

Mutex* mutex1 = new Mutex();

static Task* task1 = new Task(&task1Func, nullptr, 200, 1250, 0, 900);
static Task* task2 = new Task(&task2Func, nullptr, 200, 1250, 0, 1000);
static Task* task3 = new Task(&task3Func, nullptr, 200, 1250, 0, 5000);

ISR(SCHEDULER_ISR) {
#ifdef DOTRACE
  //Serial.println("ISR");
#endif
  Sched_CtxSwitch();
  
  // the return from interrupt call must be explicitly added
  __asm__ __volatile__ ( "reti" );
}

void setup() {
  Sched_Init();

  Serial.begin(9600);
  while (!Serial) { // wait for serial port to connect
    ;
  }

  assertCond(Sched_Add(task1) == 0, F("Failed to add task 1"));
  assertCond(Sched_Add(task2) == 0, F("Failed to add task 2"));
  assertCond(Sched_Add(task3) == 0, F("Failed to add task 3"));

  Sched_Start();

  free(task1);
  free(task2);
  free(task3);

  free(mutex1);
}

void loop() {
  // do nothing
}
