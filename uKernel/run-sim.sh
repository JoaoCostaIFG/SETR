#!/bin/sh

make DEBUGBUILD=1 || exit
simavr -g -m "atmega328p" "./build-uno/uKernel_.elf"
