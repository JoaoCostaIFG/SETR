ARDUINO_DIR = /usr/share/arduino
ARDMK_DIR = /usr/share/arduino
AVR_TOOLS_DIR = /usr
AVRDUDE_CONF = /etc/avrdude.conf
ARDUINO_CORE_PATH = /usr/share/arduino/hardware/archlinux-arduino/avr/cores/arduino
ARDUINO_PLATFORM_LIB_PATH = /usr/share/arduino/hardware/archlinux-arduino/avr/libraries
BOARDS_TXT = /usr/share/arduino/hardware/archlinux-arduino/avr/boards.txt
ARDUINO_VAR_PATH = /usr/share/arduino/hardware/archlinux-arduino/avr/variants
BOOTLOADER_PARENT = /usr/share/arduino/hardware/archlinux-arduino/avr/bootloaders

BOARD_TAG    = uno
MONITOR_PORT = /dev/ttyACM0
ARDUINO_LIBS =

# if this build for debugging purposes =>
# 	compiles without optimizations + with symbols + func call tracing
DEBUG_FLAGS = -O0 -ggdb3 -DDOTRACE
# ignore util/delay.h warning about disabling optimizations
DEBUG_FLAGS += -Wno-cpp

# do runtime asserts
CPPFLAGS += -DRUNTIMEASSERT

# allocate space for canaries and check for their integrity at runtime
CPPFLAGS += -DDOCANARIES

# ignore Arduino core deprecation warnings
CPPFLAGS += -Wno-deprecated-declarations

# Source: https://github.com/sudar/Arduino-Makefile
include Arduino.mk

.PHONY: simavr
simavr:
	make DEBUG=1
	simavr -g -m $(AVRDUDE_MCU) $(TARGET_ELF)

.PHONY: format-code
format-code:
	bash -c 'clang-format -i --sort-includes src/*.h include/*.cpp'
