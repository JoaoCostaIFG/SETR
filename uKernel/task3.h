#ifndef TASK3_H
#define TASK3_H

#include "task.h"

class Task3 : public Task {
public:
  Task3(unsigned int period, unsigned int timeDelay, unsigned int prio): Task(period, timeDelay, prio) {
  }

  void init(void);

  void run(void);
};

#endif // TASK3_H
