# Makefile to build DRenderer module.

ifeq "$(RONATIVE)" "YES"
# Build DRenderer natively on RISC OS:
CC_BUILD := gcc
CFLAGS_BUILD := -std=c99 -O2 -mthrowback -mlibscl
AS_HOST := asasm
ASFLAGS_HOST := -tb
STRIP_HOST := strip
STRIPFLAGS_HOST := -O binary
CURDIR := @.
EXTMOD :=
else
# Build DRenderer using GCCSDK cross-compiler/cross-assembler
CC_BUILD := gcc
CFLAGS_BUILD := -std=c99 -O2
AS_HOST := asasm
ASFLAGS_HOST :=
STRIP_HOST := arm-unknown-riscos-strip
STRIPFLAGS_HOST := -O binary
CURDIR := ./
EXTMOD = ,ffa
endif
PREFIX ?= .

SRCDIR := $(dir $(lastword $(MAKEFILE_LIST)))

all:	DRenderer$(EXTMOD)

clean:
ifeq "$(RONATIVE)" "YES"
	ifthere o then wipe o ~CFR~V
	remove logtable
	remove makelogtable
else
	-rm *.o logtable makelogtable
endif

dist-clean: clean
ifeq "$(RONATIVE)" "YES"
	remove DRenderer$(EXTMOD)
else
	-rm DRenderer$(EXTMOD)
endif

ifneq "$(RONATIVE)" "YES"
install: DRenderer$(EXTMOD)
	mkdir -p $(PREFIX)/module/
	cp DRenderer$(EXTMOD) $(PREFIX)/module/
	chmod 755 $(PREFIX)/module/DRenderer$(EXTMOD)
endif

.PHONY: all clean dist-clean

DRenderer.o: logtable $(SRCDIR)DRenderer.s
ifeq "$(RONATIVE)" "YES"
	cdir o
endif
	$(AS_HOST) $(ASFLAGS_HOST) -PreDefine "LogTableFileName SETS "\""logtable"\""" -o $@ $(SRCDIR)DRenderer.s

DRenderer$(EXTMOD): DRenderer.o
	$(STRIP_HOST) $(STRIPFLAGS_HOST) -o $@ $<
ifeq "$(RONATIVE)" "YES"
	SetType DRenderer$(EXTMOD) Module
endif

logtable: makelogtable
	$(CURDIR)makelogtable > logtable

makelogtable: $(SRCDIR)makelogtable.c
	$(CC_BUILD) $(CFLAGS_BUILD) -o $@ $<
