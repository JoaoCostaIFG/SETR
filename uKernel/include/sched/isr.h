#ifndef UKERNEL_ISR_H
#define UKERNEL_ISR_H

// timer 1
#define SCHEDULER_ISR         TIMER1_COMPA_vect
#define SCHEDULER_TCCRXA      TCCR1A // prescaler config A
#define SCHEDULER_TCCRXB      TCCR1B // prescaler config B
#define SCHEDULER_TIMSK       TIMSK1
#define SCHEDULER_TCNT        TCNT1  // actual timer value
#define SCHEDULER_OCRXA       OCR1A // Output Compare Register
// freq (s) = CMR / (clock / prescaler) = CMR / (16MHz / prescale).
#define SCHEDULER_CMR         31250
#define SCHEDULER_PRESCALER   (1 << CS12) // 256 prescaler

#endif // UKERNEL_ISR_H
