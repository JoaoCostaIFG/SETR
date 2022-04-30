#ifndef TASK_H
#define TASK_H

#include <Arduino.h>

#define POINTER_SIZE_TYPE     uint16_t
#define BYTE_ALIGNMENT        1
#define BYTE_ALIGNMENT_MASK   ( 0x0000 )

// TODO dynamic
#define STACKDEPTH 200

typedef byte stack_t;

class Task {
private:
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

public:
  Task(unsigned int period, unsigned int timeDelay, int prio) :
      period(period), prio(prio) {
    this->timeDelay = timeDelay;
    this->ready = (timeDelay == 0);
    // alloc stack
    this->stack = new stack_t[STACKDEPTH];
    // get stack top addr
    this->stackAddr = &(this->stack[STACKDEPTH - 1]);
    this->stackAddr = (stack_t*) (((POINTER_SIZE_TYPE) this->stackAddr) & (~((POINTER_SIZE_TYPE) BYTE_ALIGNMENT_MASK)));
    // assert
    if ((((POINTER_SIZE_TYPE) this->stackAddr) & ((POINTER_SIZE_TYPE) BYTE_ALIGNMENT_MASK)) != 0UL) {
      Serial.println("Top of stack addr assert failed");
    }
  }

  virtual void init() = 0;

  virtual void run() = 0;

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
