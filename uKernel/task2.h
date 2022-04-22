#ifndef TASK2_H
#define TASK2_H

#include "task.h"

class Task2 : public Task {
public:
  Task2(unsigned int period, unsigned int timeDelay, unsigned int prio): Task(period, timeDelay, prio) {
  }

  void init(void);

  void run(void);
};

#endif // TASK2_H
