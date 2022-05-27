#ifndef UKERNEL_TASK_H
#define UKERNEL_TASK_H

#include <stddef.h>

#include "Map.h"

typedef unsigned char stack_t;

typedef void (* taskfunc_t)(void*);

typedef enum {
  READY, BLOCKED, NOT_READY, WAITING
} state_t;

class Task {
private:
  using MapType = Map<size_t, unsigned int>;
  using MapElement = MapType::MapElement;

  /** The task's code */
  taskfunc_t run;
  /** The parameter passed */
  void* params;
  /**
   * The stack's task
   * The stack where we save the task context for context switching.
   */
  stack_t* stack;
  /** The current stack top */
  stack_t* stackAddr;
  /** The minimum address of the task */
  stack_t* botStackAddr;
  /** The maximum address of the task */
  stack_t* topStackAddr;

  state_t state;

  const unsigned int period;
  unsigned int timeDelay;
  unsigned int deadline;
  MapType inheritedPriorities;

#ifdef DOCANARIES
  const static stack_t canary[];
#endif

  unsigned int sleepTime;

  void inline push2stack(stack_t pushable) __attribute__((always_inline));

  /**
   * Initialize stack as if _run_ was called and immediately interrupted.
   * Also insert stack canaries.
   * The stack grows downwards.
   */
  void initializeStack();

public:
  Task(taskfunc_t run, void* params, unsigned int stackSize,
       unsigned int period, unsigned int timeDelay, unsigned int deadline);

  Task(taskfunc_t run, void* params, unsigned int stackSize,
       unsigned int period, unsigned int timeDelay);

  ~Task();

  /**
   * We return the address of the variable pointing to the current stack
   * position (top). We could return the position if we pushed the stack pointer
   * into the stack => harder and wasteful.
   * // Save stack pointer. Not needed. We point to the stack addr variable (not
   * the first element's address) auxAddr = (POINTER_SIZE_TYPE) this->stackAddr;
   * this->push2stack((stack_t) ((axuAddr >> 8) & (POINTER_SIZE_TYPE) 0x00ff));
   * (*this->stackAddr) = (stack_t) (axuAddr & (POINTER_SIZE_TYPE) 0x00ff);
   */
  [[nodiscard]] stack_t** getStackAddr() { return &(this->stackAddr); }

  [[nodiscard]] bool areCanariesIntact() const;

  [[nodiscard]] unsigned int getPeriod() const { return this->period; }

  [[nodiscard]] unsigned int getDelay() const { return this->timeDelay; }

  void tick() {
    if (this->timeDelay > 0)
      --this->timeDelay;
  }

  void sleep(unsigned int st){
      this->sleepTime = st;
      this->setState(WAITING);
  }

  void reset() { this->timeDelay = this->period - 1; }

  /*
   * Intended unsigned int overflow.
   */
  void nextDeadline() { this->deadline += this->period; }

  [[nodiscard]] unsigned int getDeadline() const;

  void inheritPrio(size_t mutex, unsigned int dl);

  void restorePrio(size_t mutex) { this->inheritedPriorities.remove(mutex); }

  [[nodiscard]] bool isReady() const { return this->state == READY; }

  void setState(state_t newState) { this->state = newState; }

  [[nodiscard]] state_t getState() { return this->state; }

  void tickSleep() {
      --this->sleepTime;
      if(this->sleepTime == 0) this->setState(READY);
  }

  /*
   * See "Efficient EDF Implementation for Small Embedded System", by Giorgio
   * Buttazzo, et al. for the deadline comparison part.
   */
  bool operator<(const Task &o) const {
    if (!this->isReady() && o.isReady())
      return false;
    if (this->isReady() && !o.isReady())
      return true;

    return ((int) (this->getDeadline() - o.getDeadline())) < 0;
  }

  bool operator==(const Task &o) const {
    return this->isReady() == o.isReady() &&
           ((int) (this->getDeadline() - o.getDeadline())) == 0;
  }

  bool operator>(const Task &o) const {
    if (!this->isReady() && o.isReady())
      return true;
    if (this->isReady() && !o.isReady())
      return false;

    return ((int) (this->getDeadline() - o.getDeadline())) > 0;
  }
};

#endif // UKERNEL_TASK_H
