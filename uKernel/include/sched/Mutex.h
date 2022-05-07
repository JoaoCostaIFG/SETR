#ifndef UKERNEL_MUTEX_H
#define UKERNEL_MUTEX_H

#include "Task.h"
#include "Vector.h"

extern Task *volatile currTask;

/**
 * Mechanism that allows for mutual exclusion in critical regions.
 * Implementation tries to be as safe as possible: no operation results in
 * "undefined behaviour" (see documentation of @lock and @unlock).
 *
 * Calls to @lock and @unlock are critical regions. These are protected by
 * disabling interrupts globally.
 */
class Mutex {
private:
  /** The current holder of the mutex. Is nullptr when there is no holder. */
  Task *holder;
  /** Tasks wanting to acquire the mutex. */
  Vector<Task *> pretenders;

  /** Returns whether the mutex has a holder or not. */
  bool isLocked();

  /** Unlocks all tasks waiting to acquire the mutex and clears the array. */
  void readyPretenders();

public:
  Mutex();

  /**
   * Attempts to lock the mutex.
   * Returns error (1) when:
   * - If the task attempting to lock the mutex is the holder.
   * Returns success (0) when:
   * - The task acquires the mutex.
   *
   * This is a blocking call: only returns when mutex is acquired by the caller.
   */
  int lock();

  /**
   * Unlocks the mutex (signaling the pretenders).
   * Returns error (1) when:
   * - Mutex isn't locked.
   * - Or mutex is locked, but the caller isn't the holder.
   * Returns success (0) when:
   * - The current holder is the caller, thus unlocking the mutex.
   */
  int unlock();
};

#endif // UKERNEL_MUTEX_H