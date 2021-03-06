#include <string.h>
#include <stdint.h>

#include "sched/Task.h"
#include "sched/context.h"
#include "sched/assert.h"

// Arduino UNO is a 16-bit machine 1 byte memory alignment
#define POINTER_SIZE_TYPE uint16_t
#define BYTE_ALIGNMENT 1
#define BYTE_ALIGNMENT_MASK 0x0000

#ifdef DOCANARIES
#define CANARY_SIZE 3

const stack_t Task::canary[CANARY_SIZE] = {0x11, 0x22, 0x33};
#endif

Task::Task(taskfunc_t run, void* params, unsigned int stackSize,
           unsigned int period, unsigned int timeDelay, unsigned int deadline)
    : run(run), params(params), period(period), deadline(deadline) {
  this->timeDelay = timeDelay;
  this->state = (timeDelay == 0) ? READY : NOT_READY;
  if (this->state == READY) // set delay for next period
    this->reset();

  // alloc function stack + space for canaries + space for context registers
#ifdef DOCANARIES
  stackSize += CANARY_SIZE * 2;
#endif
  stackSize += N_REGS_SAVED;
  const size_t stackSizeBytes = stackSize * sizeof(stack_t);
  this->stack = (stack_t*) malloc(stackSizeBytes);
  assertCond(this->stack != nullptr, F("Failed to alloc stack"));
  memset(this->stack, 0, stackSizeBytes);

  // get stack top address
  this->botStackAddr = &(this->stack[stackSize - (POINTER_SIZE_TYPE) 1]);
  // byte align pointer
  this->botStackAddr = (stack_t*) (((POINTER_SIZE_TYPE) this->botStackAddr) &
                                   ~((POINTER_SIZE_TYPE) BYTE_ALIGNMENT_MASK));
  assertCond((((POINTER_SIZE_TYPE)this->botStackAddr &
                                  (POINTER_SIZE_TYPE) BYTE_ALIGNMENT_MASK) == 0UL),
             F("Failed to calculate bottom stack address"));

  this->topStackAddr = this->botStackAddr - stackSize;
  // initialize task's stack
  this->stackAddr = this->botStackAddr; // start at the beginning of the stack
  this->initializeStack();

  static_assert(sizeof(*this) == 28,
                "The task's data is exceeding the calculated optimal size. Did "
                "you add a new data member?");
}

Task::Task(taskfunc_t run, void* params, unsigned int stackSize,
           unsigned int period, unsigned int timeDelay)
    : Task(run, params, stackSize, period, timeDelay, period) {}

Task::~Task() { free(this->stack); }

void inline Task::push2stack(stack_t pushable) {
  *this->stackAddr = pushable;
  this->stackAddr--;
}

void Task::initializeStack() {
  this->stackAddr = this->botStackAddr; // start at the beginning of the stack

#ifdef DOCANARIES
  // The bottom canaries
  this->push2stack(canary[0]);
  this->push2stack(canary[1]);
  this->push2stack(canary[2]);
  // The top canaries
  memcpy(topStackAddr + 1, canary, 3);
#endif

  // save task code (PC) at the bottom of the stack
  auto runAddr = (POINTER_SIZE_TYPE) this->run;
  this->push2stack((stack_t) (runAddr & (POINTER_SIZE_TYPE) 0x00ff));
  runAddr >>= 8;
  this->push2stack((stack_t) (runAddr & (POINTER_SIZE_TYPE) 0x00ff));

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
  auto paramAddr = (POINTER_SIZE_TYPE) this->params;
  this->push2stack((stack_t) (paramAddr & (POINTER_SIZE_TYPE) 0x00ff));
  paramAddr >>= 8;
  this->push2stack((stack_t) (paramAddr & (POINTER_SIZE_TYPE) 0x00ff));

  /* More debug values on R26-R31 */
  this->push2stack((stack_t) 0x26); /* R26 */
  this->push2stack((stack_t) 0x27); /* R27 */
  this->push2stack((stack_t) 0x28); /* R28 */
  this->push2stack((stack_t) 0x29); /* R29 */
  this->push2stack((stack_t) 0x30); /* R30 */
  this->push2stack((stack_t) 0x31); /* R31 */
}

bool Task::areCanariesIntact() const {
#ifdef DOCANARIES
  return *botStackAddr == canary[0] && *(botStackAddr - 1) == canary[1] &&
         *(botStackAddr - 2) == canary[2] &&
         memcmp(topStackAddr + 1, canary, 3) == 0;
#else
  return true;
#endif
}

unsigned int Task::getDeadline() const {
  unsigned int dl = this->deadline;

  // find the lowest inherited deadline => highest prio
  for (size_t i = 0; i < this->inheritedPriorities.getSize(); ++i) {
    MapElement* elem = this->inheritedPriorities.at(i);
    if (elem->value < dl)
      dl = elem->value;
  }

  return dl;
}

void Task::inheritPrio(size_t mutex, unsigned int dl) {
  MapElement* elem = this->inheritedPriorities.get(mutex);
  if (elem == nullptr) {
    this->inheritedPriorities.set(mutex, dl);
  } else if (elem->value < dl) {
    elem->value = dl;
  }
}
