#include "task.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"

#define NT 20

Task tasks[NT];
unsigned int curr_task = NT + 1;

int Sched_Add(unsigned int period, unsigned int delay, void (*func)(void), unsigned prio) {
    if (!tasks[prio].func) {
            tasks[prio] = {
                period,
                delay,
                func,
                (delay == 0)
            };
            return prio;
    }
    return -1;
}

void Sched_Dispatch() {
    int prev_task = curr_task;

    for (int i = 0; i < prev_task; ++i) {
        if (tasks[i].func && tasks[i].exec) {
            tasks[i].exec = 0;
            
            // run task
            curr_task = i;
            interrupts();
            tasks[i].func();
            noInterrupts();
            curr_task = prev_task;

            // delete one-shot
            if (tasks[i].period == 0) {
                tasks[i].func = 0;
            }
        }
    }
}

void Sched_Schedule() {
    for (int i = 0; i < NT; ++i ) {
        if (!tasks[i].func || tasks[i].exec)
            continue;

        if (tasks[i].delay == 0) {
            tasks[i].exec = 1;
            tasks[i].delay = tasks[i].period + 1;
        }
        --tasks[i].delay;
    }
}

ISR(TIMER1_COMPA_vect) {
    Sched_Schedule();
    Sched_Dispatch();
}

void setup() {
  initTask1();
  Sched_Add(10, 5, task1, 0);
  initTask2();
  Sched_Add(4,  0, task2, 1);
  initTask3();
  Sched_Add(1,  0, task3, 2);

  noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
 
  OCR1A = 31250; // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts
}

void loop() {
  // do nothing 
}
