# Makefile for Shared Object Manager module v2
# Copyright 2006, 2007 GCCSDK Developers

CC = arm-unknown-riscos-gcc
CFLAGS = -O3 -mmodule -Wall -fno-strict-aliasing

OBJCOPY = arm-unknown-riscos-objcopy

CMUNGE = cmunge
CMUNGEFLAGS = -tgcc -32bit -p

SRCS = som_init.c som_final.c som_command.c som_swihandler.c som_main.c som_os_swis.c som_alloc.c \
som_register.c link_list.c som_utilswis.c

all: SOManager,ffa

clean:
	-rm somanager.o somanager.h SOManager,ffa

SOManager,ffa: $(SRCS) somanager.h somanager.o
	$(CC) $(CFLAGS) -mmodule -o SOManager,e1f $(SRCS) somanager.o
	$(OBJCOPY) -O binary SOManager,e1f $@
	rm SOManager,e1f

# CMunge invocation:
somanager.h somanager.o: somanager.cmhg
	$(CMUNGE) $(CMUNGEFLAGS) somanager.cmhg -o somanager.o -d somanager.h
