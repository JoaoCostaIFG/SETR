#include <Arduino.h>

#include "sched/Mutex.h"
#include "sched/scheduler.h"

Mutex::Mutex() { this->holder = nullptr; }

int Mutex::lock() {
  noInterrupts();
  if (!this->isLocked()) {
    this->holder = currTask;
    interrupts();
    return 0;
  }

  // current owner can't lock same mutex again - error
  if (this->holder == currTask) {
    interrupts();
    return 1;
  }

  this->pretenders.push(currTask);

  // blocking task inherits priority of the blocked task
  this->holder->inheritPrio((size_t)this, currTask->getDeadline());

  interrupts();

  Sched_Block();

  // tail recursive - stack does not grow ad nauseam
  return this->lock();
}

int Mutex::unlock() {
  noInterrupts();

  int ret = 1;
  if (this->isLocked() && this->holder == currTask) {
    this->holder->restorePrio((size_t)this);
    this->holder = nullptr;
    ret = 0;

    readyPretenders();
  }

  interrupts();

  return ret;
}

bool Mutex::isLocked() { return this->holder != nullptr; }

void Mutex::readyPretenders() {
  for (size_t i = 0; i < this->pretenders.getSize(); ++i) {
    this->pretenders[i]->setState(READY);
  }
  this->pretenders.clear(); // clear pretenders
}
