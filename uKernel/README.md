# µKernel

This is a real-time micro-kernel for the
[Arduino UNO](https://www.arduino.cc/en/Main/arduinoBoardUno).

This project was developed for a university curricular unit about real-time
systems. It was a collaboration between:

- [Davide Castro](https://github.com/davide-castro20);
- [Henrique Ribeiro](https://github.com/TheZambi);
- [João Costa](https://github.com/JoaoCostaIFG);

## Code functionalities

Our kernel implementation allows for many functionalities such as:

- **Mutexes**: The mutex implementation can be found
  [here](./src/sched/Mutex.cpp). These mutexes have the same functionalities as
  normal C mutexes, assuming the user is using the mutex type
  `PTHREAD_MUTEX_ERRORCHECK`.
- **Earliest Deadline First With Priority Inheritance**: Taking advantage of the
  implemented mutexes, the kernel assigns priorities according to EDF with
  priority inheritance. To keep the priority up-to-date, an implementation of a
  map was made. This is used to map each mutex used to the earliest deadline of
  any task that needs it, facilitating the priority inheritance.
- **Sleep**: When using the sleep function, the task is put in a "WAITING"
  state, not being allowed to execute until the sleep is done.
- **Assert**: In order to better debug the code and enforce some conditions, the
  kernel supports assertions made by the user.
- **Stack Canaries**: See [below](#-DDOCANARIES).

## Building and Uploading

This project uses a modified version of the
[Arduino-Makefile](https://github.com/sudar/Arduino-Makefile) project as the
build system. The files from that project (along with their modifications) are
included in the [ardmk directory](./ardmk).

### Requirements

In order to build and deploy/upload the code to an Arduino UNO, the following
requirements are needed:

- **avr-gcc** - compiler;
- **avr-libc** - standard library;
- **arduino** - arduino library;
- **binutils**;
- **avrdude** (optional) - push code to the Arduino;

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

- Location of the Arduino directories (e.g. libs, boards.txt);
- (optional) The port where the device is available (for uploading the code to
  the device).

For this purpose, some example Makefiles have been included: one for Arch Linux
based systems, and another for Ubuntu based (more specifically,
[WLS 2](https://docs.microsoft.com/en-us/windows/wsl/install)).

A user wanting to compile this project, should copy one of this example files,
and (if needed) adapt it to their system: you'll most likely only need to change
the serial port where the arduino is connected.

### Conditional compilation regions

There are some conditional compilation zones in the code that can be toggled
with compiler flags. Have a look at [Makefile.linux](./uKernel.mk) for more
information.

#### -DDOTRACE

The kernel will emit messages (through serial communication) when the code
executes _main_ methods, for example, it will emit a message every time ISR,
Yield, Block, Sleep are called.

By default, this is only active when compiling for debug: `make DEBUG=1`.

#### -DRUNTIMEASSERT

The kernel will perform asserts during run-time about critical conditions, where
failure is considered unrecoverable, for example:

- Integrity of the tasks' stacks;
- Success of memory allocation;
- Out-of-bounds vector accesses.

When any of these asserts fails, the code emits a message (through serial
communication) about the error, including information about where in the code it
happened (file, function, line), and goes into a _locked_ where no more task
execution takes place.  
When on this state, the device periodically blinks its **builtin LED** spelling
out _SOS_ in morse code.

This is enabled by default.

#### -DDOCANARIES

The kernel will emit code that reserves some bytes at the beginning and at the
end of each task's stack, and fills them with predetermined values. The
integrity of these values is checked every time a task _yields_ (finishes
execution for a period).

Although not fool-proof, this can help detect situations where the chosen stack
size for a task isn't enough.

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
