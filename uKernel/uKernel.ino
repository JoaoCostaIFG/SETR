#include "include/assert.h"
#include "include/scheduler.h"
#include "include/mutex.h"
#include "include/task1.h"
#include "include/task2.h"
#include "include/task3.h"
#include "include/task4.h"

Mutex* mutex1 = Sched_CreateMutex();

static Task* task1 = new Task(&task1Func, (void*) 0, 43, 3, 0, 3);
static Task* task2 = new Task(&task2Func, (void*) 0, 43, 3, 0, 3);
static Task* task3 = new Task(&task3Func, (void*) 0, 36, 3, 0, 3);
//static Task* task4 = new Task(&task4Func, (void*) 0, 43, 1, 0, 1);

ISR(SCHEDULER_ISR, ISR_NAKED) __attribute__ ((hot, flatten));

ISR(SCHEDULER_ISR) {
#ifdef DEBUG
  Serial.println("ISR");
#endif
  Sched_CtxSwitch();

  // the return from interrupt call must be explicitly added
  __asm__ __volatile__ ( "reti" );
}

void setup() {
  Sched_Init();

#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect
    ;
  }
#endif

  assert(Sched_Add(task1) == 0);
  assert(Sched_Add(task2) == 0);
  assert(Sched_Add(task3) == 0);

  Sched_Start();

  free(task1);
  free(task2);
  free(task3);
  //free(task4);

  free(mutex1);
}

void loop() {
  // do nothing
}
