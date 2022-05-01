#include "include/task.h"

Task::Task(taskfunc_t run, void* params, unsigned int period, unsigned int timeDelay, int prio) :
    run(run), params(params), period(period), prio(prio) {
  this->timeDelay = timeDelay;
  this->ready = (timeDelay == 0);
  // alloc stack
  //this->stack = new stack_t[STACKDEPTH];
  this->stack = (stack_t*) malloc(STACKDEPTH * sizeof(stack_t));
  memset(this->stack, 0, STACKDEPTH * sizeof(stack_t));
  // get stack top addr
  this->stackAddr = &(this->stack[STACKDEPTH - (uint16_t) 1]);
  this->stackAddr = (stack_t*) (((POINTER_SIZE_TYPE)
      this->stackAddr) & ~((POINTER_SIZE_TYPE) BYTE_ALIGNMENT_MASK));
  this->initializeStack();
}

void inline push2stack(stack_t** stack, stack_t pushable) {
  **stack = pushable;
  --(*stack);
}

void Task::initializeStack() {
  POINTER_SIZE_TYPE usAddress;

  /* Place a few bytes of known values on the bottom of the stack.
  This is just useful for debugging. */
  push2stack(&this->stackAddr, 0x11);
  push2stack(&this->stackAddr, 0x22);
  push2stack(&this->stackAddr, 0x33);

  // save task code (bottom of the stack)
  usAddress = (POINTER_SIZE_TYPE) (&this->run);
  push2stack(&this->stackAddr, (stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff));
  usAddress >>= 8;
  push2stack(&this->stackAddr, (stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff));

  /* Next simulate the stack as if after a call to SAVE_CONTEXT().
  SAVE_CONTEXT places the flags on the stack immediately after r0
  to ensure the interrupts get disabled as soon as possible, and so ensuring
  the stack use is minimal should a context switch interrupt occur. */
  push2stack(&this->stackAddr, (stack_t) 0x00); /* R0 */
  push2stack(&this->stackAddr, (stack_t) 0x80);

  /* Now the remaining registers. The compiler expects R1 to be 0. */
  push2stack(&this->stackAddr, (stack_t) 0x00); /* R1 */
  /* Place debug values in R2-R23 */
  push2stack(&this->stackAddr, (stack_t) 0x02); /* R2 */
  push2stack(&this->stackAddr, (stack_t) 0x03); /* R3 */
  push2stack(&this->stackAddr, (stack_t) 0x04); /* R4 */
  push2stack(&this->stackAddr, (stack_t) 0x05); /* R5 */
  push2stack(&this->stackAddr, (stack_t) 0x06); /* R6 */
  push2stack(&this->stackAddr, (stack_t) 0x07); /* R7 */
  push2stack(&this->stackAddr, (stack_t) 0x08); /* R8 */
  push2stack(&this->stackAddr, (stack_t) 0x09); /* R9 */
  push2stack(&this->stackAddr, (stack_t) 0x10); /* R10 */
  push2stack(&this->stackAddr, (stack_t) 0x11); /* R11 */
  push2stack(&this->stackAddr, (stack_t) 0x12); /* R12 */
  push2stack(&this->stackAddr, (stack_t) 0x13); /* R13 */
  push2stack(&this->stackAddr, (stack_t) 0x14); /* R14 */
  push2stack(&this->stackAddr, (stack_t) 0x15); /* R15 */
  push2stack(&this->stackAddr, (stack_t) 0x16); /* R16 */
  push2stack(&this->stackAddr, (stack_t) 0x17); /* R17 */
  push2stack(&this->stackAddr, (stack_t) 0x18); /* R18 */
  push2stack(&this->stackAddr, (stack_t) 0x19); /* R19 */
  push2stack(&this->stackAddr, (stack_t) 0x20); /* R20 */
  push2stack(&this->stackAddr, (stack_t) 0x21); /* R21 */
  push2stack(&this->stackAddr, (stack_t) 0x22); /* R22 */
  push2stack(&this->stackAddr, (stack_t) 0x23); /* R23 */

  /* Place the parameter on the stack in the expected location. */
  usAddress = (POINTER_SIZE_TYPE) this->params;
  push2stack(&this->stackAddr, (stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff));
  usAddress >>= 8;
  push2stack(&this->stackAddr, (stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff));

  /* More debug values on R26-R31 */
  push2stack(&this->stackAddr, (stack_t) 0x26); /* R26 */
  push2stack(&this->stackAddr, (stack_t) 0x27); /* R27 */
  push2stack(&this->stackAddr, (stack_t) 0x28); /* R28 */
  push2stack(&this->stackAddr, (stack_t) 0x29); /* R29 */
  push2stack(&this->stackAddr, (stack_t) 0x30); /* R30 */
  push2stack(&this->stackAddr, (stack_t) 0x31); /* R31 */
}

void Task::stackDump() {
  uint16_t runAddr = (POINTER_SIZE_TYPE) (&this->run);
  Serial.print("Run addr: ");
  Serial.println(runAddr, 16);

  Serial.println("Stack:");
  for (int i = 0; i < STACKDEPTH; ++i) {
    Serial.println(this->stack[i], 16);
  }
}