#include <Arduino.h>

#include "include/scheduler.h"
#include "include/context.h"
#include <stdlib.h>

#define NT 20

// TODO delete one-shot

void idleTaskFunc(void* arg) {
  while (true) {
#ifdef DEBUG
    //Serial.println("In idle");
    //delay(1000);
#endif
    ;
  }
};

// tasks
static Task* tasks[NT]; // lower index => higher task priority
static int nTasks = 0;

//TODO: max_int
static Task* idleTask = new Task(&idleTaskFunc, (void*) 0, 128, 1, 0, 999);
volatile Task* curr_task;

// stack
volatile TCB_t* volatile currentStack = nullptr;

void Sched_SetupTimer() {
  noInterrupts();

  /* configure time */
  SCHEDULER_TCCRXA = 0;
  SCHEDULER_TCCRXB = 0;
  SCHEDULER_TCNT = 0;
  // freq (s) = CMR / (clock / prescaler) = CMR / (16MHz / prescale).
  SCHEDULER_OCRXA = 31250;                // Compare Match Register (CMR)
  SCHEDULER_TCCRXB |= SCHEDULER_PRESCALER;  // 256 prescaler
  SCHEDULER_TCCRXB |= (1 << WGM12);         // CTC mode
  SCHEDULER_TIMSK |= (1 << OCIE1A);        // enable timer compare interrupt

  // TODO ?
  interrupts();
}

void Sched_Init() {
  for (auto& task: tasks)
    task = nullptr;

  Sched_Add(idleTask);
}

void sortTasks() {
  qsort(tasks, nTasks, sizeof(Task*), compareTask);
}

void Sched_Start() {
#ifdef DEBUG
  Serial.println("Start");
#endif

  sortTasks();
  currentStack = tasks[0]->getStackAddr();

  Sched_SetupTimer();

  // restore context of the first task to run
  noInterrupts();
  RESTORE_CONTEXT();

  /* Simulate a function call end as generated by the compiler. We will now
  jump to the start of the task the context of which we have just restored. */
  __asm__ __volatile__ ( "ret" );

  /* UNREACHABLE */
  return;
}

//TODO: assert nTasks < NT
void Sched_Add(Task* t) {
  tasks[nTasks++] = t;
}


void Sched_Dispatch() {
#ifdef DEBUG
  //Serial.println("Dispatch");
#endif

  Task* highestTaskPrio = tasks[0];

  if (highestTaskPrio && highestTaskPrio != curr_task) {
    curr_task = highestTaskPrio;
    currentStack = highestTaskPrio->getStackAddr(); // set current stack
  }
}

int Sched_Schedule() {
#ifdef DEBUG
  //Serial.println("Schedule");
#endif

  idleTask->setReady(true);

  int readyCnt = 0;
  for (int i = 0; i < nTasks; ++i) {
    Task* t = tasks[i];
    if (!t) continue;

    if (t->isReady()) { // ready tasks are already scheduled
      ++readyCnt;
    }

    if (t->getDelay() > 0) {
      t->tick();
    } else {
      t->setReady(true);
      ++readyCnt;
      t->reset();
    }
  }

  // sort priorities
  sortTasks();

  return readyCnt;
}

void Sched_YieldDispatch() {
#ifdef DEBUG
  Serial.println("Yield");
#endif

  curr_task->setReady(false);
  curr_task->nextDeadline();
  sortTasks();
  curr_task = nullptr; // can go to any task

  Sched_Dispatch();
}

void Sched_Yield() {
  SAVE_CONTEXT(); // save the execution context

  // dispatch
  Sched_YieldDispatch();

  RESTORE_CONTEXT(); // restore the execution context

  // the return from function call must be explicitly added
  __asm__ __volatile__ ( "ret" );
}

void Sched_CtxSwitch() {
  SAVE_CONTEXT(); // save the execution context

  // sched + dispatch
  if (Sched_Schedule() > 1) {
    Sched_Dispatch();
  }

  RESTORE_CONTEXT(); // restore the execution context

  // the return from function call must be explicitly added
  __asm__ __volatile__ ( "ret" );
}
