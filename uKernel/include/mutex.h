#ifndef MUTEX_H
#define MUTEX_H

#include "task.h"

#define N_PRETS 20 - 1 // TODO: dynamic memory (vector)

extern volatile Task* currTask;

class Mutex {
private:
  volatile Task* holder;
  volatile Task* pretenders[N_PRETS];

  bool isLocked();

  int insertPretender(volatile Task* pretender);

  void readyPretenders();

public:
  Mutex();

  int lock();

  int unlock();
};


#endif // MUTEX_H