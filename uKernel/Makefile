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
MCU_TYPE 		 = atmega328p
ARDUINO_LIBS =

# enable warnings
CFLAGS += -Wall -Werror
CPPFLAGS += -Wall -Werror

# ignore Arduino core deprecation warnings
CFLAGS += -Wno-deprecated-declarations
CPPFLAGS += -Wno-deprecated-declarations

# do runtime asserts
CFLAGS += -DRUNTIMEASSERT
CPPFLAGS += -DRUNTIMEASSERT

# allocate space for canaries and check for their integrity at runtime
CFLAGS += -DDOCANARIES
CPPFLAGS += -DDOCANARIES

# is this build for debugging purposes => compiles with symbols and does trace
DEBUGBUILD ?= 0
ifeq ($(DEBUGBUILD), 1)
	CFLAGS += -DDOTRACE -ggdb3
	CPPFLAGS += -DDOTRACE -ggdb3
endif

# Source: https://github.com/sudar/Arduino-Makefile
include Arduino.mk

.PHONY: simavr
simavr:
	make DEBUGBUILD=1
	simavr -g -m $(MCU_TYPE) "./build-uno/uKernel_.elf"

.PHONY: format-code
format-code:
	fish -c 'clang-format -i --sort-includes **/*{.cpp,.h}'
