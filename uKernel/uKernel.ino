// vim:filetype=cpp:tw=80:et
#include "task.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"

#define NT 20

Task* task1 = new Task(&initTask1, &runTask1, 10, 5, 7);
//Task2 task2(initTask2, runTask2, 4, 0, 1);
Task* task3 = new Task(&initTask3, &runTask3, 1, 0, 2);

Task* tasks[NT]; // lower int => higher task priority
unsigned int curr_task = NT + 1;

void Sched_Init() {
  for (auto & task : tasks)
    task = nullptr;

  /* configure time */
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  //OCR1A = 6250;   // compare match register 16MHz/256/10Hz
  OCR1A = 31250;  // compare match register 16MHz/256/2Hz
  //OCR1A = 31;       // compare match register 16MHz/256/2kHz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts  
}

int Sched_Add(Task* t) {
  const int prio = 0;//t->getPrio();
  if (!tasks[prio]) {
    t->init();
    tasks[prio] = t;
    return prio;
  }
  return -1;
}

void Sched_Dispatch() {
  /*
  unsigned int prev_task = curr_task;

  for (unsigned int i = 0; i < prev_task; ++i) {
    Task* t = tasks[i];
    if (!t || !t->isReady())
      continue;

    t->setReady(false);

    // run task
    curr_task = i;
    interrupts();
    t->run();
    noInterrupts();
    curr_task = prev_task;

    // delete one-shot
    if (t->getPeriod() == 0) {
      tasks[i] = nullptr;
    }
  }
  */

  //for (int i = 0; i < NT; ++i) {
  //  Task* t = tasks[i];
  //  if (t == nullptr)
  //    continue;

  //  if (t->isReady() && i == 0) {
  //    t->setReady(false);
  //  }
  //}
}

void Sched_Schedule() {
  for (int i = 0; i < NT; ++i) {
    Task* t = tasks[i];
    if (i != 0) continue;
    task3->run();
    delay(1000);
    task3->run();
    delay(1000);
  }

  /*
  for (int i = 0; i < NT; ++i) {
    Task* t = tasks[i];

    if (!t || t->isReady()) // ready tasks are already scheduled
      continue;

    if (t->getDelay() > 0) {
      t->tick();
    } else {
      t->setReady(true);
      t->reset();
    }
  }
  */
}

ISR(TIMER1_COMPA_vect) {
  //Sched_Schedule();
  //Sched_Dispatch();
}

void setup() {
  Sched_Init();

  task3->init();

  //Sched_Add(&task1);
  task1->init();
  tasks[0] = task1;

  //Sched_Add(&task2);
  //Sched_Add(&task3);
}

void loop() {
  // do nothing 
}
