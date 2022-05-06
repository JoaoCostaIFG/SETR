#ifndef UKERNEL_SCHEDULER_H
#define UKERNEL_SCHEDULER_H

#include "Task.h"
#include "scheduler_isr.h"

void Sched_Init();

void Sched_Start();

void Sched_Stop();

int Sched_Add(Task *t);

void Sched_Dispatch();

int Sched_Schedule();

// the same as CtxSwitch but without the scheduler call (it's not a tick)
void Sched_Yield() __attribute__((hot, naked));

void Sched_CtxSwitch() __attribute__((hot, flatten, naked));

void Sched_BlockTask() __attribute__((naked));

#endif // UKERNEL_SCHEDULER_H
