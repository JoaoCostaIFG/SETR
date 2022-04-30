#include <stdlib.h>

#include "include/task.h"

Task::Task(taskfunc_t run, void* params, unsigned int period, unsigned int timeDelay, int prio) :
    run(run), params(params), period(period), prio(prio) {
  this->timeDelay = timeDelay;
  this->ready = (timeDelay == 0);
  // alloc stack
  //this->stack = new stack_t[STACKDEPTH];
  this->stack = (stack_t*) malloc(STACKDEPTH * sizeof(stack_t));
  // get stack top addr
  this->stackAddr = &(this->stack[STACKDEPTH - (uint16_t) 1]);
  this->stackAddr = (stack_t*) (((POINTER_SIZE_TYPE)
      this->stackAddr) & ~((POINTER_SIZE_TYPE) BYTE_ALIGNMENT_MASK));
  // assert
  if ((((POINTER_SIZE_TYPE) this->stackAddr) & ((POINTER_SIZE_TYPE) BYTE_ALIGNMENT_MASK)) != 0UL) {
    Serial.println("Top of stack addr assert failed");
  }
  this->initializeStack();
}

void Task::initializeStack() {
  POINTER_SIZE_TYPE usAddress;

  // save task code (bottom of the stack)
  usAddress = (POINTER_SIZE_TYPE) (&this->run);
  *this->stackAddr = (stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff);
  this->stackAddr--;
  usAddress >>= 8;
  *this->stackAddr = (stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff);
  this->stackAddr--;

  /* Next simulate the stack as if after a call to SAVE_CONTEXT().
  SAVE_CONTEXT places the flags on the stack immediately after r0
  to ensure the interrupts get disabled as soon as possible, and so ensuring
  the stack use is minimal should a context switch interrupt occur. */
  *this->stackAddr = (stack_t) 0x00;    /* R0 */
  this->stackAddr--;
  *this->stackAddr = (stack_t) 0x80;
  this->stackAddr--;

  /* Now the remaining registers. The compiler expects R1 to be 0. */
  *this->stackAddr = (stack_t) 0x00;    /* R1 */

  /* Leave R2 - R23 untouched */
  this->stackAddr -= 23;

  /* Place the parameter on the stack in the expected location. */
  usAddress = (POINTER_SIZE_TYPE) this->params;
  *this->stackAddr = (stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff);
  this->stackAddr--;
  usAddress >>= 8;
  *this->stackAddr = (stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff);

  /* Leave register R26 - R31 untouched */
  this->stackAddr -= 7;
}