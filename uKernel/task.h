#ifndef TASK_H
#define TASK_H

class Task {
protected:
  const unsigned int period;
  const unsigned int prio;

  unsigned int timeDelay;
  bool ready;

public:
  Task(unsigned int period, unsigned int timeDelay, unsigned int prio) : period(period), prio(prio) {
    this->timeDelay = timeDelay;
    this->ready = (timeDelay == 0);
  }

  virtual void init(void) = 0;

  virtual void run(void) = 0;

  unsigned int getPeriod() const {
    return this->period;
  }

  unsigned int getDelay() const {
    return this->timeDelay;
  }

  void setDelay(unsigned int timeDelay) {
    this->timeDelay = timeDelay;
  }

  unsigned int getPrio() const {
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
