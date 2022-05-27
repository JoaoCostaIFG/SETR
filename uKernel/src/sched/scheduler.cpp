#include <Arduino.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "sched/Task.h"
#include "Vector.h"
#include "sched/context.h"
#include "sched/assert.h"
#include "sched/scheduler.h"

#define MAXTIMEDIFF (UINT_MAX / 2)

void idleTaskFunc(void* arg);

static bool preempt = true;
// tasks
static Vector<Task*> tasks; // lower index => higher task priority
Task* volatile currTask;
// idle task
static Task* idleTask =
    new Task(&idleTaskFunc, nullptr, 80, 1, 0, MAXTIMEDIFF - 1);
// stack
volatile stackPtr_t* volatile currStack = nullptr;

/** This task never yields. As such, when it starts running, it stays always ready. */
void idleTaskFunc(void* arg) {
  while (true) {
#ifdef DOTRACE
    // Serial.println("In idle");
#endif
    // delay(1000);
    ;
  }
}

void Sched_Preempt() {
  preempt = true;
}

void Sched_NoPreempt() {
  preempt = false;
}

int Sched_Add(Task* t) {
  if (t->getPeriod() >= MAXTIMEDIFF)
    return 1;
  tasks.push(t);
  return 0;
}

void Sched_Init() { Sched_Add(idleTask); }

static void Sched_SetupTimer() {
  noInterrupts();

  /* configure time */
  SCHEDULER_TCCRXA = 0;
  SCHEDULER_TCCRXB = 0;
  SCHEDULER_TCNT = 0;
  // freq (s) = CMR / (clock / prescaler) = CMR / (16MHz / prescale).
  SCHEDULER_OCRXA = SCHEDULER_CMR;                 // Compare Match Register (CMR)
  SCHEDULER_TCCRXB |= SCHEDULER_PRESCALER; // 256 prescaler
  SCHEDULER_TCCRXB |= (1 << WGM12);        // CTC mode
  SCHEDULER_TIMSK |= (1 << OCIE1A);        // enable timer compare interrupt

  interrupts();
}

static int compareTasks(const void* a, const void* b) {
  Task* t1 = *((Task**) a);
  Task* t2 = *((Task**) b);

  // push idle task to the end of the ready part (between the ready and
  // not-ready parts)
  if (t1 == idleTask)
    return t2->isReady() ? 1 : -1;
  if (t2 == idleTask)
    return t1->isReady() ? -1 : 1;

  if (*t1 < *t2)
    return -1;
  if (*t1 > *t2)
    return 1;

  // this is done to enforce order between tasks with the same deadline after
  // sorting. if this was not done, some tasks, even though they had the same
  // prio, could preempt tasks with the same piority, which is not desired
  return (size_t) t1 < (size_t) t2;
}

static inline void Sched_SortTasks() {
  tasks.sort(compareTasks);
}

static inline void Sched_SetCurrTask(Task* newCurrTask) {
  currTask = newCurrTask;
  currStack = newCurrTask->getStackAddr(); // set current stack
}

static void Sched_Dispatch() {
#ifdef DOTRACE
  // Serial.println("Dispatch");
#endif

  Sched_SortTasks(); // sort priorities

  Task* nextTask = tasks[0]; // highest priority task
  if (nextTask && nextTask->isReady() && nextTask != currTask) {
    // only switch to ready tasks that aren't the current one
    Sched_SetCurrTask(nextTask);
  }
}

void Sched_Start() {
#ifdef DOTRACE
  Serial.println("Start");
#endif

  // select next running task
  Sched_Dispatch();

  // this is a tick-based kernel
  Sched_SetupTimer();

  // restore context of the first task to run
  noInterrupts(); // needs the interrupts to be disabled
  RESTORE_CONTEXT();
  // Simulate a function call end as generated by the compiler.
  // We will now jump to the start of the task the context of which we have just
  // restored.
  __asm__ __volatile__("ret");

  /* UNREACHABLE */
  return;
}

void Sched_Stop() {
  noInterrupts();

  // disable timer compare interrupt
  SCHEDULER_TIMSK &= ~(1 << OCIE1A);

  interrupts();
}

static int Sched_Schedule() {
#ifdef DOTRACE
  // Serial.println("Schedule");
#endif

  int readyCnt = 0;
  for (size_t i = 0; i < tasks.getSize(); ++i) {
    Task* t = tasks[i];
    // count ready tasks
    readyCnt += (int) t->isReady();

    if (t->getDelay() > 0) {
      t->tick();
    } else {
      if (t->getState() != BLOCKED) {
        ++readyCnt;
        t->setState(READY);
      }
      t->reset();
    }
    if (t->getState() == WAITING) {
      t->tickSleep();
    }
  }

  // if preemption is disabled, return "no tasks to switch to"
  return readyCnt * (int) preempt;
}

char str[25];

void Sched_CtxSwitch() {
  SAVE_CONTEXT(); // save the execution context

  sprintf(str, "%ld", micros());
  Serial.println(str);

  // sched + dispatch
  if (Sched_Schedule() > 1) {
    Sched_Dispatch();
  }

  sprintf(str, "%ld", micros());
  Serial.println(str);

  RESTORE_CONTEXT(); // restore the execution context

  // the return from function call must be explicitly added
  __asm__ __volatile__("ret");
}

static void Sched_YieldDispatch() {
#ifdef DOTRACE
  Serial.println("Yield");
#endif

  assertCond(currTask->areCanariesIntact(), F("Canaries are broken"));

  if (currTask->getPeriod() == 0) {
    // delete one-shot tasks
    tasks.remove(currTask);
  } else {
    currTask->setState(NOT_READY);
    currTask->nextDeadline();
  }
  currTask = nullptr; // can go to any task

  Sched_Dispatch();
}

void Sched_Yield() {
  SAVE_CONTEXT(); // save the execution context

  // dispatch
  Sched_YieldDispatch();

  RESTORE_CONTEXT(); // restore the execution context

  // the return from function call must be explicitly added
  __asm__ __volatile__("ret");
}

static void Sched_BlockDispatch() {
#ifdef DOTRACE
  Serial.println("Block");
#endif

  currTask->setState(BLOCKED);
  currTask = nullptr;
  Sched_Dispatch();
}

void Sched_Block() {
  SAVE_CONTEXT(); // save the execution context

  Sched_BlockDispatch();

  RESTORE_CONTEXT(); // restore the execution context

  __asm__ __volatile__("ret");
}

static void Sched_SleepDispatch(unsigned int sleepTime) {
#ifdef DOTRACE
  Serial.println("Sleep");
#endif
  currTask->sleep(sleepTime);
  currTask = nullptr;
  Sched_Dispatch();
}

void Sched_Sleep(unsigned int sleepTime) {
  SAVE_CONTEXT(); // save the execution context

  Sched_SleepDispatch(sleepTime);

  RESTORE_CONTEXT(); // restore the execution context

  __asm__ __volatile__("ret");
}
