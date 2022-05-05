#ifndef TASK_H
#define TASK_H

#include <Arduino.h>

typedef byte stack_t;

typedef void (*taskfunc_t)(void *);

typedef enum { READY, BLOCKED, NOT_READY, WAITING } state_t;

class Task {
private:
  /** The task's code */
  taskfunc_t run;
  /** The parameter passed */
  void *params;
  /**
   * The stack's task
   * The stack where we save the task context for context switching.
   */
  stack_t *stack;
  /** The current stack top */
  stack_t *stackAddr;
  /** The minimum address of the task */
  stack_t *botStackAddr;
  /** The maximum address of the task */
  stack_t *topStackAddr;

  const unsigned int period;
  volatile unsigned int timeDelay;
  volatile unsigned int deadline;
  volatile unsigned int currentDeadline;
  volatile state_t state;

  const static stack_t canary[];

  void inline push2stack(stack_t pushable) __attribute__((always_inline));

  /**
   * Initialize stack as if _run_ was called and immediately interrupted.
   * Also insert stack canaries.
   * The stack grows downwards.
   */
  void initializeStack();

public:
  Task(taskfunc_t run, void *params, unsigned int stackSize,
       unsigned int period, unsigned int timeDelay, unsigned int deadline);

  Task(taskfunc_t run, void *params, unsigned int stackSize,
       unsigned int period, unsigned int timeDelay);

  ~Task();

  /**
   * We return the adress of the variable pointing to the current stack position
   * (top). We could return the position if we pushed the stack pointer into the
   * stack => harder and wasteful.
   * // Save stack pointer. Not needed. We point to the stack addr variable (not
   * the first element's address) auxAddr = (POINTER_SIZE_TYPE) this->stackAddr;
   * this->push2stack((stack_t) ((axuAddr >> 8) & (POINTER_SIZE_TYPE) 0x00ff));
   * (*this->stackAddr) = (stack_t) (axuAddr & (POINTER_SIZE_TYPE) 0x00ff);
   */
  stack_t **getStackAddr() { return &(this->stackAddr); }

  bool areCanariesIntact() const volatile;

  unsigned int getPeriod() const { return this->period; }

  unsigned int getDelay() const { return this->timeDelay; }

  void tick() {
    if (this->timeDelay > 0)
      --this->timeDelay;
  }

  void reset() { this->timeDelay = this->period - 1; }

  /*
   * Intended unsigned int overflow.
   */
  void nextDeadline() volatile { this->deadline += this->period; }

  unsigned int getDeadline() const { return this->deadline; }

  unsigned int getCurrentDeadline() const { return this->currentDeadline; }

  void setCurrentDeadline(unsigned int newDeadline) volatile { this->currentDeadline = newDeadline; }

  void setOriginalDeadline() volatile { this->currentDeadline = this->deadline; }

  bool isReady() const { return this->state == READY; }

  void setState(state_t state) volatile { this->state = state; }

  state_t getState() { return this->state; }

  /*
   * See "Efficient EDF Implementation for Small Embedded System", by Giorgio
   * Buttazzo, et al. for the deadline comparison part.
   */
  bool operator<(const Task &o) const {
    if (!this->isReady() && o.isReady())
      return false;
    if (this->isReady() && !o.isReady())
      return true;

    return ((int)(this->getDeadline() - o.getDeadline())) < 0;
  }

  bool operator==(const Task &o) const {
    return this->isReady() == o.isReady() &&
           ((int)(this->getDeadline() - o.getDeadline())) == 0;
  }

  bool operator>(const Task &o) const {
    if (!this->isReady() && o.isReady())
      return true;
    if (this->isReady() && !o.isReady())
      return false;

    return ((int)(this->getDeadline() - o.getDeadline())) > 0;
  }
};

#endif // TASK_H
