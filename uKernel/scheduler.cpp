#include <Arduino.h>
#include <limits.h>

#include "include/scheduler.h"

#include "include/assert.h"
#include "include/context.h"
#include "include/task.h"

// TODO periods have to be lower than this number
#define MAXTIMEDIFF UINT_MAX/2
#define NT 20

// TODO delete one-shot

void idleTaskFunc(void* arg);

// tasks
static int nTasks = 0;
static Task* tasks[NT]; // lower index => higher task priority
static volatile Task* currTask;
// idle task
static Task* idleTask = new Task(&idleTaskFunc, (void*) 0, 80, 1, 0, MAXTIMEDIFF - 1);
// stack
volatile TCB_t* volatile currStack = nullptr;

void idleTaskFunc(void* arg) {
  while (true) {
#ifdef DEBUG
    //Serial.println("In idle");
#endif
    //delay(1000);
    ;
  }
}

void Sched_SetupTimer() {
  noInterrupts();

  /* configure time */
  SCHEDULER_TCCRXA = 0;
  SCHEDULER_TCCRXB = 0;
  SCHEDULER_TCNT = 0;
  // freq (s) = CMR / (clock / prescaler) = CMR / (16MHz / prescale).
  SCHEDULER_OCRXA = 31250;                  // Compare Match Register (CMR)
  SCHEDULER_TCCRXB |= SCHEDULER_PRESCALER;  // 256 prescaler
  SCHEDULER_TCCRXB |= (1 << WGM12);         // CTC mode
  SCHEDULER_TIMSK |= (1 << OCIE1A);         // enable timer compare interrupt
}

void Sched_Init() {
  for (auto &task: tasks)
    task = nullptr;

  Sched_Add(idleTask);
}

int compareTask(const void* a, const void* b) {
  Task* t1 = *((Task**) a);
  Task* t2 = *((Task**) b);

  // push idle task to the end of the ready part (between the ready and not-ready parts)
  if (t1 == idleTask) return t2->isReady() ? 1 : -1;
  if (t2 == idleTask) return t1->isReady() ? -1 : 1;

  if (*t1 < *t2) return -1;
  if (*t1 > *t2) return 1;

  // this is done to enforce order between tasks with the same deadline after sorting.
  // if this was not done, some tasks, even though they had the same prio, could preempt tasks with the
  // same piority, which is not desired
  return (size_t) t1 < (size_t) t2;
}

void Sched_SortTasks() {
  qsort(tasks, nTasks, sizeof(Task*), compareTask);
}

void Sched_SetCurrTask(Task* newCurrTask) {
  currTask = newCurrTask;
  currStack = newCurrTask->getStackAddr(); // set current stack
}

void Sched_Start() {
#ifdef DEBUG
  Serial.println("Start");
#endif

  Sched_SortTasks();
  Sched_SetCurrTask(tasks[0]);

  Sched_SetupTimer();

  // restore context of the first task to run
  RESTORE_CONTEXT();
  // Simulate a function call end as generated by the compiler.
  // We will now jump to the start of the task the context of which we have just restored.
  __asm__ __volatile__ ( "ret" );

  /* UNREACHABLE */
  return;
}

//TODO: assert nTasks < NT
void Sched_Add(Task* t) {
  tasks[nTasks++] = t;
  assert(nTasks < NT);
}

int Sched_Schedule() {
#ifdef DEBUG
  //Serial.println("Schedule");
#endif

  int readyCnt = 0;
  for (int i = 0; i < nTasks; ++i) {
    Task* t = tasks[i];
    if (!t) continue;

    if (t->isReady()) ++readyCnt;

    if (t->getDelay() > 0) {
      t->tick();
    } else {
      ++readyCnt;
      t->setReady(true);
      t->reset();
    }
  }

  // sort priorities
  Sched_SortTasks();

  return readyCnt;
}

void Sched_Dispatch() {
#ifdef DEBUG
  //Serial.println("Dispatch");
#endif

  Task* nextTask = tasks[0];
  if (nextTask && nextTask->isReady() && nextTask != currTask) {
    Sched_SetCurrTask(nextTask);
  }
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

// Yielding (finished execution)
void Sched_YieldDispatch() {
#ifdef DEBUG
  Serial.println("Yield");
#endif

  assert(currTask->areCanariesIntact());

  currTask->setReady(false);
  currTask->nextDeadline();
  Sched_SortTasks();
  currTask = nullptr; // can go to any task

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
