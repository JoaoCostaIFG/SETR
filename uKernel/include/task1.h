#ifndef TASK1_H
#define TASK1_H

#include "task.h"

class Task1 : public Task {
public:
  Task1(unsigned int period, unsigned int timeDelay, int prio) :
      Task(period, timeDelay, prio) {}

  void init() override;

  void run() override;
};

#endif // TASK1_H
