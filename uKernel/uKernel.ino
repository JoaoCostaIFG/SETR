#include "include/scheduler.h"
#include "include/task1.h"
#include "include/task2.h"
#include "include/task3.h"
#include "include/task4.h"

static Task* task1 = new Task(&task1Func, (void*) 0, 69, 3, 0, 3);
static Task* task2 = new Task(&task2Func, (void*) 0, 69, 3, 0, 3);
static Task* task3 = new Task(&task3Func, (void*) 0, 69, 3, 0, 3);
//static Task* task4 = new Task(&task4Func, (void*) 0, 128, 1, 0, 1);

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

  Sched_Add(task1);
  Sched_Add(task2);
  Sched_Add(task3);

  Sched_Start();
}

void loop() {
  // do nothing
}
