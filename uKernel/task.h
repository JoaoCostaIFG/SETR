#ifndef TASK_H
#define TASK_H

class Task {
private:
  void (* initFunc)();

  void (* runFunc)();

  const unsigned int period;
  const int prio;

  unsigned int timeDelay;
  bool ready;

public:
  Task(void (* initFunc)(), void (* runFunc)(), unsigned int period, unsigned int timeDelay, int prio) :
      initFunc(initFunc), runFunc(runFunc), period(period), prio(prio) {
    this->timeDelay = timeDelay;
    this->ready = (timeDelay == 0);
  }

  Task() : Task(nullptr, nullptr, 0, 0, 0) {}

  virtual void init() {
    this->initFunc();
  }

  virtual void run() {
    this->runFunc();
  }

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
