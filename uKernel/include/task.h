#ifndef TASK_H
#define TASK_H

class Task {
private:
  const unsigned int period;
  const int prio;

  volatile unsigned int timeDelay;
  volatile bool ready;

public:
  /**
   * The stack where we save the task context for context switching.
   */
  volatile byte stack[200];

  Task(unsigned int period, unsigned int timeDelay, int prio) :
      period(period), prio(prio) {
    this->timeDelay = timeDelay;
    this->ready = (timeDelay == 0);
  }

  virtual void init() = 0;

  virtual void run() = 0;

  unsigned int getPeriod() const {
    return this->period;
  }

  unsigned int getDelay() const {
    return this->timeDelay;
  }

  void tick() {
    if (this->timeDelay > 0)
      --this->timeDelay;
  }

  void reset() {
    this->timeDelay = this->period - 1;
  }

  int getPrio() const {
    return this->prio;
  }

  bool isReady() const {
    return this->ready;
  }

  void setReady(bool isReady) {
    this->ready = isReady;
  }
};

#endif // TASK_H
