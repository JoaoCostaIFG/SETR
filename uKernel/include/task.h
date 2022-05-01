#ifndef TASK_H
#define TASK_H

#include <Arduino.h>

#define POINTER_SIZE_TYPE     uint16_t
#define BYTE_ALIGNMENT        1
#define BYTE_ALIGNMENT_MASK   ( 0x0000 )

// TODO dynamic
#define STACKDEPTH 500

typedef byte stack_t;

typedef void (* taskfunc_t)(void*);

class Task {
private:
  taskfunc_t run;
  void* params;

  const unsigned int period;
  const int prio;

  volatile unsigned int timeDelay;
  volatile bool ready;
  /**
   * The task's stack
   * The stack where we save the task context for context switching.
   */
  stack_t* stack;

  stack_t* stackAddr;

  // Initialize stack as if _run_ was called and immediately interrupted
  void initializeStack();

  bool firstStart = false;

public:
  Task(taskfunc_t run, void* params, unsigned int period, unsigned int timeDelay, int prio);

  void stackDump();

  stack_t* getStackAddr() {
    return this->stackAddr;
  }

  unsigned int getPeriod() const {
    return this->period;
  }

  unsigned int getDelay() const {
    return this->timeDelay;
  }

  void tick() {
    if (this->timeDelay > 0)
      --this->timeDelay;
  }

  void reset() {
    this->timeDelay = this->period - 1;
  }

  int getPrio() const {
    return this->prio;
  }

  bool isReady() const {
    return this->ready;
  }

  void setReady(bool isReady) {
    this->ready = isReady;
  }
};

#endif // TASK_H
