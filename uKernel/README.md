# µKernel

This is a real-time micro-kernel for the
[Arduino UNO](https://www.arduino.cc/en/Main/arduinoBoardUno).

This project was developed for an university curricular unit about real-time
systems. It was a colaboration between:

- [Davide Castro](https://github.com/davide-castro20);
- [Henrique Ribeiro](https://github.com/TheZambi);
- [João Costa](https://github.com/JoaoCostaIFG);

## Code functionalities

Our kernel implementation allows for many functionalities such as:
- **Mutexes**: The mutex implementation can be found [here](./src/sched/Mutex.cpp). These 
mutexes have the same functionalities as normal C mutexes, 
assuming the user is using the mutex type 
"PTHREAD_MUTEX_ERRORCHECK".
- **Earliest Deadline First With Priority Inheritance**: Taking advantage
 of the implemented mutexes, the kernel assigns priorities according 
 to EDF with priority inheritance. To keep the priority up-to-date, 
 an implementation of a map was made. This is used to map each 
 mutex used to the earliest deadline of any task that needs it, 
 facilitating the priority inheritance.
- **Sleep**: When using the sleep function, the task is put in 
a "WAITING" state, not being allowed to execute until the sleep is done.
- **Assert**: In order to better debug the code and enforce 
some conditions, the kernel supports assertions made by the user.
- **Stack Canaries**: See below.

## Building and Uploading

This project uses a modified version of the
[Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) project as the
build system.

### Requirements

In order to build and deploy/upload the code to an Arduino UNO, the following
requirements are needed:

- **avr-gcc** - compiler;
- **avr-libc** - standard library;
- **arduino** - arduino library;
- **arduino-mk** - build system;
- **binutils**;
- **avrdude** - push code to the Arduino;

### About Arduino IDE

The [Arduino IDE](https://archlinux.org/packages/community/x86_64/arduino/) does
not allow us to organize the project's files in different directories, use
different compiler flags, and the group finds it an all-around bad solution.

We've kept the _main_ file as a Arduino IDE sketch file (`.ino` file), so the
project can be opened in the IDE. Although the IDE can open the project, it
isn't able to compile it, but we can still use things like the IDE's
[Serial Monitor](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-monitor),

### Makefile

The build system needs the user to define some information about the system:

- Location of the Arduino libs;
- The board identification;
- The port where it is connected.

For this purpose, some example Makefiles have been included: one for Arch Linux
based systems, and another for Ubuntu based (more specifically,
[WLS 2](https://docs.microsoft.com/en-us/windows/wsl/install)).

A user wanting to compile this project, should copy one of this example files,
and possibly adapt it to their system: you'll most likely only need to change
the serial port where the arduino is connected.

### Compiler flags

There are some conditional compilation zones in the code that can be toggled
with compiler flags. Have a look at [Makefile.linux](./uKernel.mk) for more
information.

## Debugging

For debugging purposes, the group made extensive use of
[simavr](https://github.com/buserror/simavr) in conjunction with
[avr-gdb](https://github.com/embecosm/avr-binutils-gdb).

- The make target `simavr` builds the code with debug symbols and optimizations
  turned off;
- The make target `gdb` starts avr-gdb. The project includes a
  [gdb init file](./gdbinit) which attempts to automatically connect to the
  running sim avr instance (`target remote :1234`). This only works if you
  allows project specific auto-loading init files for gdb.
