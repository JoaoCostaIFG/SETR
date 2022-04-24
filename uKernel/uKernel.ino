// vim:filetype=cpp:tw=80:et
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "regs.h"

#define NT 20

static Task* task1 = new Task1(3, 1, 0);
static Task* task2 = new Task2(1, 0, 1);
static Task* task3 = new Task3(1, 0, 2);
static Task* task4 = new Task4(1, 0, 3);

// tasks
Task* tasks[NT + 1]; // lower int => higher task priority
volatile unsigned int curr_task = NT;

// stacks
byte baseStack[200];
volatile TCB_t* volatile pxCurrentTCB = baseStack;

void Sched_Init() {
  for (auto& task: tasks)
    task = nullptr;

  /* configure time */
  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // clock_speed / prescaler_value - 1.
  //OCR1A = 6250;   // compare match register 16MHz/256/10Hz
  OCR1A = 31250;  // compare match register 16MHz/256/2Hz
  //OCR1A = 31;       // compare match register 16MHz/256/2kHz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  //interrupts(); // enable all interrupts
}

int Sched_Add(Task* t) {
  int prio = t->getPrio();
  if (!tasks[prio]) {
    t->init();
    tasks[prio] = t;
    return prio;
  }
  return -1;
}

void Sched_Dispatch() {
  Serial.println("Dispatch");

  unsigned int prev_task = curr_task;

  for (unsigned int i = 0; i < prev_task; ++i) {
    Task* t = tasks[i];
    if (!t || !t->isReady())
      continue;

    t->setReady(false);

    // run task
    curr_task = i;
    pxCurrentTCB = t->stack; // set current stack
    interrupts();
    t->run();
    noInterrupts();
    curr_task = prev_task;

    // delete one-shot
    if (t->getPeriod() == 0) {
      tasks[i] = nullptr;
    }
  }

  if (prev_task < NT && tasks[prev_task]) {
    pxCurrentTCB = tasks[prev_task]->stack;
  } else {
    pxCurrentTCB = baseStack;
  }
}

int Sched_Schedule() {
  Serial.println("Schedule");

  int readyCnt = 0;

  for (int i = 0; i < NT; ++i) {
    Task* t = tasks[i];
    if (!t) continue;

    if (t->isReady()) { // ready tasks are already scheduled
      ++readyCnt;
      continue;
    }

    if (t->getDelay() > 0) {
      t->tick();
    } else {
      t->setReady(true);
      ++readyCnt;
      t->reset();
    }
  }

  return readyCnt;
}

//void handleISR() __attribute__(( naked ));

void handleISR() {
  /* Macro that explicitly saves the execution context. */
  //portSAVE_CONTEXT();

  // handle ISR
  if (Sched_Schedule() > 0) {
    Sched_Dispatch();
  }

  /* Macro that explicitly restores the execution context. */
  //portRESTORE_CONTEXT();

  // the return from function call must be explicitly added
  //__asm__ __volatile__ ( "ret" );
}

//ISR(portSCHEDULER_ISR, ISR_NAKED) __attribute__ (( signal ));

ISR(TIMER1_COMPA_vect) {
  handleISR();

  // the return from interrupt call must be explicitly added
  //__asm__ __volatile__ ( "reti" );
}

void setup() {
  Sched_Init();

  Serial.begin(9600);
  while (!Serial) { ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  Sched_Add(task1);
  Sched_Add(task2);
  Sched_Add(task3);

  Serial.println("Start");

  interrupts();
}

void loop() {
  // do nothing
}
