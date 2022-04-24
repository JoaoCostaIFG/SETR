#ifndef TASK3_H
#define TASK3_H

#include "task.h"

class Task3 : public Task {
public:
  Task3(unsigned int period, unsigned int timeDelay, int prio) :
      Task(period, timeDelay, prio) {}

  void init() override;

  void run() override;
};

#endif // TASK3_H
