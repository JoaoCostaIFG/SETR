# uKernel DevLog #1

I'm currently enrolled in a class about embedded and real-time systems. For this
class's final project, I'm developing a real-time kernel for
[Arduino UNO](https://www.arduino.cc/en/Main/arduinoBoardUno). I'll try to
document the development in a series of posts. This is part #1.

On the last chapter, we've talked about basic concepts of real-time systems.
Now, we'll talk about implementing multiple stacks, one for each task.

## Why multiple stacks?

There are two reaons for us to want to implement a system where each task has
its own stack:

1. In the future, we'll implement mutexes which can lead to tasks being in a
   blocked state (priority inversion);
2. We want our task functions to follow the prototypes of POSIX thread
   functions: `void* task(void* arg)`.

On this devlog, we'll focus on the second one.

### The new task functions

Originally, a task for toggling an LED could look something like this:

```c
// toggles an LED
void runTask() {
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}
```

This function would be called for each activation of the task. Alongside this
function, we would also benefit from hacing a _setup_ function for each task,
that would be called whenever the task is created:

```c
// setup the LED pin as OUTPUT and turn the LED on
void setupTask() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
}
```

The plan is to join these methods (and the multiple calls to `runTask`) into a
single method, that is called once:

```c
void taskFunc(void *arg) {
  // set pin as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH); // turn on LED

  while (true) {
    // toggle LED
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));

    Sched_Yield(); // finished execution for this activation
  }
}
```

In order for the function to only be called once, we wrap the contents that need
to run on each activation into a `while(true)` loop.  
**Note:** contrary to what POSIX threads, we return void from these functions,
because they aren't meant to _end_.
