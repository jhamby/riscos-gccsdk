# Makefile for the GNU Compiler Collection for ARM/RISC OS
# Copyright (c) 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

CC = $(standard_exec_prefix)/gcc -O2 $(OPTS) $(CFL) -I.
GPC_FLAGS=--unit-path=units --automake

libgpcobjdir=$(objdir)/gcc-$(VERSION)/libgpc/$(TRG)

GPCRTS_LIB = $(libgpcobjdir)/rts.o \
	$(libgpcobjdir)/write.o \
	$(libgpcobjdir)/read.o \
	$(libgpcobjdir)/file.o \
	$(libgpcobjdir)/rts-va.o \
	$(libgpcobjdir)/bind.o \
	$(libgpcobjdir)/randfile.o \
	$(libgpcobjdir)/pexecute.o \
	$(libgpcobjdir)/internal.o \
	$(libgpcobjdir)/string.o \
	$(libgpcobjdir)/error.o \
	$(libgpcobjdir)/time.o \
	$(libgpcobjdir)/random.o \
	$(libgpcobjdir)/string2.o \
	$(libgpcobjdir)/filename.o \
	$(libgpcobjdir)/files.o \
	$(libgpcobjdir)/getopt.o \
	$(libgpcobjdir)/numtodec.o \
	$(libgpcobjdir)/sets.o \
	$(libgpcobjdir)/heap.o \
	$(libgpcobjdir)/math.o \
	$(libgpcobjdir)/endian.o \
	$(libgpcobjdir)/move.o \
	$(libgpcobjdir)/init.o \
	$(libgpcobjdir)/gpc.o


all:	$(TBIN)/$(TRG)/libgpc.o

$(TBIN)/$(TRG)/libgpc.o: $(GPCRTS_LIB)
	$(standard_exec_prefix)/libfile -c $@ $(GPCRTS_LIB)


# GNU Pascal standard run-time library
rts/rts-config.inc: rts/rts-config.h
	sed -e 's,/\*,{,g;s,\*/,},g;s,^[ \t]*#\(.*\),{$$\1},' < rts/rts-config.h > $@

$(libgpcobjdir)/rts.o: rts/rts.c
	$(CC) -o $@ -c $< -I. -I./rts 

$(libgpcobjdir)/write.o: rts/write.c
	$(CC) -o $@ -c $< -I. -I./rts 

$(libgpcobjdir)/read.o: rts/read.c
	$(CC) -o $@ -c $< -I. -I./rts 

$(libgpcobjdir)/file.o: rts/file.c
	$(CC) -o $@ -c $< -I. -I./rts 

$(libgpcobjdir)/rts-va.o: rts/rts-va.c
	$(CC) -o $@ -c $< -I. -I./rts 

$(libgpcobjdir)/bind.o: rts/bind.c
	$(CC) -o $@ -c $< -I. -I./rts 

$(libgpcobjdir)/randfile.o: rts/randfile.c
	$(CC) -o $@ -c $< -I. -I./rts 

$(libgpcobjdir)/pexecute.o: rts/pexecute.c
	$(CC) -o $@ -c $< -I. -I./rts 

$(libgpcobjdir)/internal.o: rts/internal.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/string.o: rts/string.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/error.o: rts/error.pas rts/rts-config.inc
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/time.o: rts/time.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/random.o: rts/random.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/string2.o: rts/string2.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/filename.o: rts/filename.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/files.o: rts/files.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/getopt.o: rts/getopt.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/numtodec.o: rts/numtodec.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/sets.o: rts/sets.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/heap.o: rts/heap.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/math.o: rts/math.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/endian.o: rts/endian.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/move.o: rts/move.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

$(libgpcobjdir)/init.o: rts/init.pas
	$(CC) -o $@ -c $< -I. -I./rts -DRTS_RELEASE_STRING=\"20000813\" $(GPC_FLAGS)

$(libgpcobjdir)/gpc.o: rts/gpc.pas
	$(CC) -o $@ -c $< -I. -I./rts $(GPC_FLAGS)

