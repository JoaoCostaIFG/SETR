ARDMK_DIR = ./ardmk
BOARD_TAG = uno

STD 				= c
STDVERSION	= 17

CFLAGS_STD = -std=$(STD)$(STDVERSION) -flto -fno-fat-lto-objects
CXXFLAGS_STD = -std=$(STD)++$(STDVERSION) -fno-threadsafe-statics -flto

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
include $(ARDMK_DIR)/Arduino.mk

.PHONY: simavr
simavr:
	make DEBUG=1
	simavr -g -m $(AVRDUDE_MCU) $(TARGET_ELF)

.PHONY: gdb
gdb:
	avr-gdb $(TARGET_ELF)

.PHONY: format-code
format-code:
	bash -c 'clang-format -i --sort-includes --style=LLVM include/*.h src/*.cpp'
