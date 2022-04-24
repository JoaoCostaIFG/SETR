#ifndef TASK4_H
#define TASK4_H

#include "task.h"

class Task4 : public Task {
public:
  Task4(unsigned int period, unsigned int timeDelay, int prio) :
      Task(period, timeDelay, prio) {}

  void init() override;

  void run() override;
};

#endif // TASK4_H
