#ifndef UKERNEL_MUTEX_H
#define UKERNEL_MUTEX_H

#include "Task.h"
#include "Vector.h"

extern Task *volatile currTask;

class Mutex {
private:
  Task *holder;
  Vector<Task *> pretenders;

  bool isLocked();

  void readyPretenders();

public:
  Mutex();

  int lock();

  int unlock();
};

#endif // UKERNEL_MUTEX_H