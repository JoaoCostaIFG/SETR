#define LEDPIN1 13
#define LEDPIN2 12
#define LEDPIN3 11

#define BUTTON1 A1
#define BUTTON2 A2
#define BUTTON3 A3

#define NT 20

typedef struct 
{
    unsigned int period;
    unsigned int delay;
    void (*func)(void);
    unsigned int exec;
} Task;

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

volatile int button1St = 0;
volatile int button2St = 0;
volatile int button3St = 0;

void FuncX() {
    digitalWrite(LEDPIN1, HIGH);
    delay(1000);
    digitalWrite(LEDPIN1, LOW);
}

void FuncY() {
    digitalWrite(LEDPIN2, HIGH);
    delay(500);
    digitalWrite(LEDPIN2, LOW);
}

void FuncZ() {
    digitalWrite(LEDPIN3, HIGH);
    delay(100);
    digitalWrite(LEDPIN3, LOW);
}

ISR(TIMER1_COMPA_vect) {
    Sched_Schedule();
    Sched_Dispatch();
}

void setup(){
  //set pin as output
  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);

  Sched_Add(10, 5, FuncX, 0);
  Sched_Add(4,  0, FuncY, 1);
  Sched_Add(1,  0, FuncZ, 2);

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

void loop(){
  // do nothing
}
