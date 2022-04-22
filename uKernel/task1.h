#ifndef TASK1_H
#define TASK1_H

#include "task.h"

class Task1 : public Task {
public:
  Task1(unsigned int period, unsigned int timeDelay, unsigned int prio): Task(period, timeDelay, prio) {
  }

  void init(void);

  void run(void);
};

#endif // TASK1_H
