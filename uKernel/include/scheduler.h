#ifndef UKERNEL_SCHEDULER_H
#define UKERNEL_SCHEDULER_H

#include "Task.h"
#include "scheduler_isr.h"

/**
 * Add a task to the scheduler.
 * Returns 0 on success (task is valid). 1, otherwise.
 */
int Sched_Add(Task* t);

/** 'Constructor' of the scheduler. */
void Sched_Init();

/** Start the scheduler (call after adding all needed tasks). */
void Sched_Start();

/** Stop the scheduler. Called on assertion failure. */
void Sched_Stop();

/**
 * Saves the running task's context, and then schedules + dispatches the next task (highest prio).
 * The idle task (always ready) is run when there are no tasks ready to run.
 * This is meant to be called from an ISR, so the next task can be the same as the interrupted.
 */
void Sched_CtxSwitch() __attribute__((hot, naked));

/**
 * Tasks should call this when they finish execution for a given period.
 * The same as Sched_CtxSwitch but without the scheduler call (it's not a tick).
 */
void Sched_Yield() __attribute__((hot, naked));

/**
 * Called when a task is blocked (priority inversion).
 * This is used by the Mutexes' lock call.
 */
void Sched_Block() __attribute__((naked));

#endif // UKERNEL_SCHEDULER_H
