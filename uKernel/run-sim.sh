#!/bin/sh

make DEBUGBUILD=1
simavr -g -m "atmega328p" "./build-uno/uKernel_.elf"
