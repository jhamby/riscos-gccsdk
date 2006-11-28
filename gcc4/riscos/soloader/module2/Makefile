# Makefile for Shared Object Manager module v2
# Copyright 2006 GCCSDK Developers

# Currently we need GCCSDK 3.4.6 cross-compiler.  We can't use GCCSDK 4.x
# for module building yet.
CC = arm-riscos-aof-gcc
CFLAGS = -mlibscl -mmodule

CMUNGE = cmunge
CMUNGEFLAGS = -tgcc -32bit -p

SRCS = som_init.c som_final.c som_command.c som_swihandler.c

all: SOManager,ffa

clean:
	-rm somanager.o somanager.h SOManager,ffa

SOManager,ffa: $(SRCS) somanager.h somanager.o
	$(CC) $(CFLAGS) -o $@ $(SRCS) somanager.h somanager.o

# CMunge invocation:
somanager.h somanager.o: somanager.cmhg
	$(CMUNGE) $(CMUNGEFLAGS) somanager.cmhg -o somanager.o -d somanager.h
