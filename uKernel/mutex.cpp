#include <Arduino.h>
#include "include/mutex.h"
#include "include/scheduler.h"


Mutex::Mutex() {
    this->holder = nullptr;
}

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

    this->insertPretender(currTask);

    interrupts();

    Sched_BlockTask();

    // tail recursive - stack does not grow ad nauseam
    return this->lock();
}


int Mutex::unlock() {
    noInterrupts();

    int ret = 1;
    if (this->isLocked() && this->holder == currTask) {
        this->holder = nullptr;
        ret = 0;
        readyPretenders();
    }

    interrupts(); // TODO: enable interrupts before readyPretenders?

    return ret;
}


bool Mutex::isLocked() {
    return this->holder != nullptr;
}

int Mutex::insertPretender(Task* pretender) {
    for (int i = 0; i < N_PRETS; ++i) {
        if (this->pretenders[i] == nullptr) {
            this->pretenders[i] = pretender;
            return 0;
        }
    }
    return 1;
}

void Mutex::readyPretenders() {
    for (int i = 0; i < N_PRETS; ++i) {
        if (this->pretenders[i] != nullptr) {
            this->pretenders[i]->setState(READY);
        }
    }
}