#ifndef TASK_H
#define TASK_H

class Task {
private:
  const unsigned int period;
  const int prio;

  unsigned int timeDelay;
  bool ready;

public:
  Task(unsigned int period, unsigned int timeDelay, int prio) : period(period), prio(prio) {
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
