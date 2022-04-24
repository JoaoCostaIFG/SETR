#ifndef UKERNEL_SCHEDULER_H
#define UKERNEL_SCHEDULER_H

#include "task.h"

void Sched_Init();

int Sched_Add(Task* t);

void Sched_Dispatch();

int Sched_Schedule();

void handleISR() __attribute__(( hot, flatten, naked ));

#endif //UKERNEL_SCHEDULER_H
