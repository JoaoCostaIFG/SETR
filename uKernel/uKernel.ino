#include <Arduino.h>

#include "sched/assert.h"
#include "sched/scheduler.h"
#include "sched/Mutex.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

// Mutex* mutex1 = new Mutex();

static Task* task1 = new Task(&task1Func, nullptr, 70, 125, 0);
static Task* task2 = new Task(&task2Func, nullptr, 70, 375, 0);
static Task* task3 = new Task(&task1Func, nullptr, 70, 200, 0);
static Task* task4 = new Task(&task2Func, nullptr, 70, 250, 0);
static Task* task5 = new Task(&task1Func, nullptr, 70, 125, 0);
static Task* task6 = new Task(&task2Func, nullptr, 70, 375, 0);
static Task* task7 = new Task(&task1Func, nullptr, 70, 125, 0);
static Task* task8 = new Task(&task2Func, nullptr, 70, 375, 0);
static Task* task9 = new Task(&task1Func, nullptr, 70, 125, 0);
static Task* task10 = new Task(&task2Func, nullptr, 70, 375, 0);
static Task* task11 = new Task(&task1Func, nullptr, 80, 125, 0);
static Task* task12 = new Task(&task2Func, nullptr, 80, 375, 0);
static Task* task13 = new Task(&task1Func, nullptr, 80, 125, 0);
static Task* task14 = new Task(&task2Func, nullptr, 80, 375, 0);
static Task* task15 = new Task(&task1Func, nullptr, 80, 125, 0);
// static Task* task3 = new Task(&task3Func, (void*) 0, 128, 3, 0, 3);
// static Task* task4 = new Task(&task4Func, (void*) 0, 43, 1, 0, 1);

ISR(SCHEDULER_ISR) {
  Serial.print("1: ");
  Serial.println(micros());
#ifdef DOTRACE
  // Serial.println("ISR");
#endif
  Sched_CtxSwitch();

  Serial.print("2: ");
  Serial.println(micros());
  
  // the return from interrupt call must be explicitly added
  __asm__ __volatile__ ( "reti" );
}

void setup() {
  Sched_Init();

  Serial.begin(9600);
  while (!Serial) { // wait for serial port to connect
    ;
  }

  assertCond(Sched_Add(task1) == 0, F("Failed to add task 1"));
  assertCond(Sched_Add(task2) == 0, F("Failed to add task 2"));
  assertCond(Sched_Add(task3) == 0, F("Failed to add task 1"));
  assertCond(Sched_Add(task4) == 0, F("Failed to add task 2"));
  assertCond(Sched_Add(task5) == 0, F("Failed to add task 1"));
  assertCond(Sched_Add(task6) == 0, F("Failed to add task 2"));
  assertCond(Sched_Add(task7) == 0, F("Failed to add task 1"));
  assertCond(Sched_Add(task8) == 0, F("Failed to add task 2"));
  assertCond(Sched_Add(task9) == 0, F("Failed to add task 1"));
  assertCond(Sched_Add(task10) == 0, F("Failed to add task 2"));
  assertCond(Sched_Add(task11) == 0, F("Failed to add task 1"));
  assertCond(Sched_Add(task12) == 0, F("Failed to add task 2"));
  assertCond(Sched_Add(task13) == 0, F("Failed to add task 1"));
  assertCond(Sched_Add(task14) == 0, F("Failed to add task 2"));
  assertCond(Sched_Add(task15) == 0, F("Failed to add task 1"));
  //assertCond(Sched_Add(task3) == 0, F("Failed to add task 3"));

  Sched_Start();

  free(task1);
  free(task2);
  free(task3);
  free(task4);
  free(task5);
  free(task6);
  free(task7);
  free(task8);
  free(task9);
  free(task10);
  free(task11);
  free(task12);
  free(task13);
  free(task14);
  free(task15);

  // free(mutex1);
}

void loop() {
  // do nothing
}
