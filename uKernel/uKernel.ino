// vim:filetype=cpp:tw=80:et
#include "task.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "regs.h"

#define NT 20

Task1 task1(10, 5, 1);
//Task2 task2(4, 0, 1);
Task3 task3(1, 0, 2);

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
  const int prio = t->getPrio();
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
  Sched_Add(&task1);
  for (int i = 0; i < NT; ++i) {
    Task* t = tasks[i];
    if (t && i == 0) {
      task3.run();
      delay(1000);
    }
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

void handleISR() {
  Sched_Schedule();
  //Sched_Dispatch();
}

//ISR(TIMER1_COMPA_vect) __attribute__((signal, naked));

ISR(TIMER1_COMPA_vect) {
  /* Macro that explicitly saves the execution context. */
  //SAVE_CONTEXT();

  // handle ISR
  handleISR();

  /* Macro that explicitly restores the execution context. */
  //RESTORE_CONTEXT();

  /* The return from interrupt call must also the explicitly added. */
  //asm volatile ( "reti" );
}

void setup() {
  task3.init();

  Sched_Add(&task1);

  //Sched_Add(&task2);
  //Sched_Add(&task3);

  Sched_Init();
}

void loop() {
  // do nothing 
}
