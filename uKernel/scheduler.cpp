#include <Arduino.h>

#include "include/scheduler.h"
#include "include/regs.h"

#define NT 20

// tasks
Task* tasks[NT]; // lower int => higher task priority
volatile unsigned int curr_task = NT;

// stacks
byte baseStack[64]; // TODO is this needed?
volatile TCB_t* volatile pxCurrentTCB = baseStack;

void Sched_Init() {
  for (auto& task: tasks)
    task = nullptr;

  /* configure time */
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // clock_speed / prescaler_value - 1.
  //OCR1A = 6250;   // compare match register 16MHz/256/10Hz
  OCR1A = 31250;  // compare match register 16MHz/256/2Hz
  //OCR1A = 31;       // compare match register 16MHz/256/2kHz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts
}

int Sched_Add(Task* t) {
  int prio = t->getPrio();
  if (!tasks[prio]) {
    t->init();
    tasks[prio] = t;
    return prio;
  }
  return -1;
}

void Sched_Dispatch() {
  Serial.println("Dispatch");

  unsigned int prev_task = curr_task;

  for (unsigned int i = 0; i < prev_task; ++i) {
    Task* t = tasks[i];
    if (!t || !t->isReady())
      continue;

    t->setReady(false);

    // run task
    curr_task = i;
    pxCurrentTCB = t->stack; // set current stack
    interrupts();
    t->run();
    noInterrupts();
    curr_task = prev_task;

    // delete one-shot
    if (t->getPeriod() == 0) {
      tasks[i] = nullptr;
    }
  }

  // restore pointer to previous stack
  pxCurrentTCB = (prev_task < NT && tasks[prev_task]) ?
                 tasks[prev_task]->stack :
                 baseStack;
}


int Sched_Schedule() {
  Serial.println("Schedule");

  int readyCnt = 0;
  for (int i = 0; i < NT; ++i) {
    Task* t = tasks[i];
    if (!t) continue;

    if (t->isReady()) { // ready tasks are already scheduled
      ++readyCnt;
      continue;
    }

    if (t->getDelay() > 0) {
      t->tick();
    } else {
      t->setReady(true);
      ++readyCnt;
      t->reset();
    }
  }

  return readyCnt;
}

void handleISR() {
  /* explicitly save the execution context */
  portSAVE_CONTEXT();

  // handle ISR
  if (Sched_Schedule() > 0) {
    Sched_Dispatch();
  }

  /* explicitly restore the execution context */
  portRESTORE_CONTEXT();

  // the return from function call must be explicitly added
  __asm__ __volatile__ ( "ret" );
}
