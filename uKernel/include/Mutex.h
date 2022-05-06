#ifndef MUTEX_H
#define MUTEX_H

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

#endif // MUTEX_H