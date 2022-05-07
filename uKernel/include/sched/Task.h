#ifndef UKERNEL_TASK_H
#define UKERNEL_TASK_H

#include <stddef.h>
#include <stdint.h>

#include "Map.h"

typedef unsigned char stack_t;

typedef void (*taskfunc_t)(void *);

typedef enum { READY, BLOCKED, NOT_READY, WAITING } state_t;

class Task {
private:
  using MapType = Map<size_t, unsigned int>;
  using MapElement = MapType::MapElement;

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

  state_t state;

  const unsigned int period;
  unsigned int timeDelay;
  unsigned int deadline;
  MapType inheritedPriorities;

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
   * We return the address of the variable pointing to the current stack
   * position (top). We could return the position if we pushed the stack pointer
   * into the stack => harder and wasteful.
   * // Save stack pointer. Not needed. We point to the stack addr variable (not
   * the first element's address) auxAddr = (POINTER_SIZE_TYPE) this->stackAddr;
   * this->push2stack((stack_t) ((axuAddr >> 8) & (POINTER_SIZE_TYPE) 0x00ff));
   * (*this->stackAddr) = (stack_t) (axuAddr & (POINTER_SIZE_TYPE) 0x00ff);
   */
  stack_t **getStackAddr() { return &(this->stackAddr); }

  bool areCanariesIntact() const;

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
  void nextDeadline() { this->deadline += this->period; }

  unsigned int getDeadline() const {
    unsigned int dl = this->deadline;

    for (size_t i = 0; i < this->inheritedPriorities.getSize(); ++i) {
      MapElement *elem = this->inheritedPriorities.at(i);
      if (elem->value > dl)
        dl = elem->value;
    }
    return dl;
  }

  void inheritPrio(size_t mutex, unsigned int dl) {
    MapElement *elem = this->inheritedPriorities.get(mutex);
    if (elem == nullptr) {
      this->inheritedPriorities.set(mutex, dl);
    } else if (elem->value < dl) {
      elem->value = dl;
    }
  }

  void restorePrio(size_t mutex) { this->inheritedPriorities.remove(mutex); }

  bool isReady() const { return this->state == READY; }

  void setState(state_t newState) { this->state = newState; }

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

#endif // UKERNEL_TASK_H
