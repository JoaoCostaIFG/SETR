#include "include/task.h"

Task::Task(taskfunc_t run, void* params, unsigned int stackSize,
           unsigned int period, unsigned int timeDelay, int prio) :
    run(run), params(params), period(period), prio(prio) {
  this->timeDelay = timeDelay;
  this->ready = (timeDelay == 0);
  // alloc stack
  size_t stackSizeBytes = stackSize * sizeof(stack_t);
  this->stack = (stack_t*) malloc(stackSizeBytes);
  memset(this->stack, 0, stackSizeBytes);
  // get stack top addr
  this->stackAddr = &(this->stack[stackSize - (uint16_t) 1]);
  this->stackAddr = (stack_t*) (((POINTER_SIZE_TYPE)
  this->stackAddr) &~((POINTER_SIZE_TYPE)BYTE_ALIGNMENT_MASK));
  this->initializeStack();
}

void inline Task::push2stack(stack_t pushable) {
  *this->stackAddr = pushable;
  this->stackAddr--;
}

void Task::initializeStack() {
  POINTER_SIZE_TYPE usAddress;

  /* Place a few bytes of known values on the bottom of the stack.
  This is just useful for debugging. */
  this->push2stack(0x11);
  this->push2stack(0x22);
  this->push2stack(0x33);

  // save task code (bottom of the stack)
  usAddress = (POINTER_SIZE_TYPE)
  this->run;
  this->push2stack((stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff));
  usAddress >>= 8;
  this->push2stack((stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff));

  /* Next simulate the stack as if after a call to SAVE_CONTEXT().
  SAVE_CONTEXT places the flags on the stack immediately after r0
  to ensure the interrupts get disabled as soon as possible, and so ensuring
  the stack use is minimal should a context switch interrupt occur. */
  this->push2stack((stack_t) 0x00); /* R0 */
  this->push2stack((stack_t) 0x80);

  /* Now the remaining registers. The compiler expects R1 to be 0. */
  this->push2stack((stack_t) 0x00); /* R1 */
  /* Place debug values in R2-R23 */
  this->push2stack((stack_t) 0x02); /* R2 */
  this->push2stack((stack_t) 0x03); /* R3 */
  this->push2stack((stack_t) 0x04); /* R4 */
  this->push2stack((stack_t) 0x05); /* R5 */
  this->push2stack((stack_t) 0x06); /* R6 */
  this->push2stack((stack_t) 0x07); /* R7 */
  this->push2stack((stack_t) 0x08); /* R8 */
  this->push2stack((stack_t) 0x09); /* R9 */
  this->push2stack((stack_t) 0x10); /* R10 */
  this->push2stack((stack_t) 0x11); /* R11 */
  this->push2stack((stack_t) 0x12); /* R12 */
  this->push2stack((stack_t) 0x13); /* R13 */
  this->push2stack((stack_t) 0x14); /* R14 */
  this->push2stack((stack_t) 0x15); /* R15 */
  this->push2stack((stack_t) 0x16); /* R16 */
  this->push2stack((stack_t) 0x17); /* R17 */
  this->push2stack((stack_t) 0x18); /* R18 */
  this->push2stack((stack_t) 0x19); /* R19 */
  this->push2stack((stack_t) 0x20); /* R20 */
  this->push2stack((stack_t) 0x21); /* R21 */
  this->push2stack((stack_t) 0x22); /* R22 */
  this->push2stack((stack_t) 0x23); /* R23 */

  /* Place the parameter on the stack in the expected location. */
  usAddress = (POINTER_SIZE_TYPE)
  this->params;
  this->push2stack((stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff));
  usAddress >>= 8;
  this->push2stack((stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff));

  /* More debug values on R26-R31 */
  this->push2stack((stack_t) 0x26); /* R26 */
  this->push2stack((stack_t) 0x27); /* R27 */
  this->push2stack((stack_t) 0x28); /* R28 */
  this->push2stack((stack_t) 0x29); /* R29 */
  this->push2stack((stack_t) 0x30); /* R30 */
  this->push2stack((stack_t) 0x31); /* R31 */

  // Save stack pointer
  // Not needed because we point to the stack address (not the first element's address)
  //usAddress = (POINTER_SIZE_TYPE) this->stackAddr;
  //this->push2stack((stack_t) ((usAddress >> 8) & (POINTER_SIZE_TYPE) 0x00ff));
  //*this->stackAddr = (stack_t) (usAddress & (POINTER_SIZE_TYPE) 0x00ff);
}