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
ARDUINO_LIBS =


DEBUGBUILD ?= 1
ifeq ($(DEBUGBUILD), 1)
	CFLAGS = -DDEBUG -g
	CPPFLAGS = -DDEBUG -g
endif


# Source: https://github.com/sudar/Arduino-Makefile
include Arduino.mk
