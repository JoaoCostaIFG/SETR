#ifndef TASK2_H
#define TASK2_H

#include "task.h"

class Task2 : public Task {
public:
  Task2(unsigned int period, unsigned int timeDelay, int prio) :
      Task(period, timeDelay, prio) {}

  void init() override;

  void run() override;
};

#endif // TASK2_H
