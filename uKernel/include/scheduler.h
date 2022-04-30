#ifndef UKERNEL_SCHEDULER_H
#define UKERNEL_SCHEDULER_H

#include "scheduler_isr.h"
#include "task.h"

void Sched_Init();

void Sched_Start();

int Sched_Add(Task* t);

void Sched_Dispatch();

int Sched_Schedule();

// the same as CtxSwitch but without the scheduler call (it's not a tick)
void Sched_ManualCtxSwitch() __attribute__(( naked ));

void Sched_CtxSwitch() __attribute__(( hot, flatten, naked ));

#endif //UKERNEL_SCHEDULER_H
