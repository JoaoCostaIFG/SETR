#ifndef TASK_H
#define TASK_H

#include <Arduino.h>

#define POINTER_SIZE_TYPE     uint16_t
#define BYTE_ALIGNMENT        1
#define BYTE_ALIGNMENT_MASK   ( 0x0000 )

typedef byte stack_t;

typedef void (* taskfunc_t)(void*);

class Task {
private:
  taskfunc_t run;
  void* params;

  const unsigned int period;
  unsigned int deadline;

  volatile unsigned int timeDelay;
  volatile bool ready;
  /**
   * The task's stack
   * The stack where we save the task context for context switching.
   */
  stack_t* stack;

  stack_t* stackAddr;

  void inline push2stack(stack_t pushable) __attribute__((always_inline));

  // Initialize stack as if _run_ was called and immediately interrupted
  void initializeStack();

public:
  Task(taskfunc_t run, void* params, unsigned int stackSize,
       unsigned int period, unsigned int timeDelay, int deadline);

  stack_t** getStackAddr() {
    return &(this->stackAddr);
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

  void nextDeadline() volatile {
    this->deadline += this->period; 
  }

  int getDeadline() const {
    return this->deadline;
  }

  bool isReady() const {
    return this->ready;
  }

  void setReady(bool isReady) volatile {
    this->ready = isReady;
  }

  bool operator<(const Task &t1) const {
    if(!this->ready && t1.isReady()) return false;
    return this->deadline < t1.getDeadline();
  }

  bool operator==(const Task &t1) const {
    return this->deadline == t1.getDeadline() && this->ready == t1.isReady();
  }

  bool operator>(const Task &t1) const {
    if(!this->ready && t1.isReady()) return true;
    return this->deadline > t1.getDeadline();
  }
};

int compareTask(const void *a, const void *b);

#endif // TASK_H
