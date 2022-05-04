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
  volatile unsigned int timeDelay;
  volatile unsigned int deadline;
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
       unsigned int period, unsigned int timeDelay, unsigned int deadline);

  Task(taskfunc_t run, void* params, unsigned int stackSize,
       unsigned int period, unsigned int timeDelay);

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

  /*
   * Intended unsigned int overflow.
   */
  void nextDeadline() volatile {
    this->deadline += this->period;
  }

  unsigned int getDeadline() const {
    return this->deadline;
  }

  bool isReady() const {
    return this->ready;
  }

  void setReady(bool isReady) volatile {
    this->ready = isReady;
  }

  /*
   * See "Efficient EDF Implementation for Small Embedded System", by Giorgio Buttazzo, et al.
   * for the deadline comparison part.
   */
  bool operator<(const Task& o) const {
    if (!this->ready && o.isReady()) return false;
    if (this->ready && !o.isReady()) return true;
    //return this->deadline < o.getDeadline();
    return ((int) (this->deadline - o.getDeadline())) < 0;
  }

  bool operator==(const Task& o) const {
    return this->ready == o.isReady() && //this->deadline == o.getDeadline();
           ((int) (this->deadline - o.getDeadline())) == 0;
  }

  bool operator>(const Task& o) const {
    if (!this->ready && o.isReady()) return true;
    if (this->ready && !o.isReady()) return false;
    //return this->deadline > o.getDeadline();
    return ((int) (this->deadline - o.getDeadline())) > 0;
  }
};

#endif // TASK_H
