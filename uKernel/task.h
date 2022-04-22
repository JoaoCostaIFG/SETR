#ifndef TASK_H
#define TASK_H

typedef struct 
{
    unsigned int period;
    unsigned int delay;
    void (*func)(void);
    unsigned int exec;
} Task;

#endif // TASK_H
