// vim:filetype=cpp:tw=81:et

#include "include/scheduler.h"
#include "include/task1.h"
#include "include/task2.h"
#include "include/task3.h"
#include "include/task4.h"

static Task* task1 = new Task1(3, 1, 0);
static Task* task2 = new Task2(1, 0, 1);
static Task* task3 = new Task3(1, 0, 2);
static Task* task4 = new Task4(1, 0, 3);

ISR(SCHEDULER_ISR, ISR_NAKED) __attribute__ ((hot, flatten));

ISR(SCHEDULER_ISR) {
  handleISR();

  // the return from interrupt call must be explicitly added
  __asm__ __volatile__ ( "reti" );
}

void setup() {
  Sched_Init();

  Serial.begin(9600);
  while (!Serial) { ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  Sched_Add(task1);
  Sched_Add(task2);
  Sched_Add(task3);

  Serial.println("Start");
}

void loop() {
  // do nothing
}
