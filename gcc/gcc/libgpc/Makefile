# Makefile for the GNU Compiler Collection for ARM/RISC OS
# Copyright (c) 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I. -I./rts
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


all:	$(ux_gcc_bin_dir)/$(TRG)/libgpc.o $(ro_gcc_bin_dir)/$(TRG)/o/libgpc

$(ux_gcc_bin_dir)/$(TRG)/libgpc.o: $(GPCRTS_LIB)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(GPCRTS_LIB)

$(ro_gcc_bin_dir)/$(TRG)/o/libgpc: $(GPCRTS_LIB)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(GPCRTS_LIB)


# GNU Pascal standard run-time library
rts/rts-config.inc: rts/rts-config.h
	sed -e 's,/\*,{,g;s,\*/,},g;s,^[ \t]*#\(.*\),{$$\1},' < rts/rts-config.h > $@

$(libgpcobjdir)/rts.o: rts/rts.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) 

$(libgpcobjdir)/write.o: rts/write.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) 

$(libgpcobjdir)/read.o: rts/read.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) 

$(libgpcobjdir)/file.o: rts/file.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) 

$(libgpcobjdir)/rts-va.o: rts/rts-va.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) 

$(libgpcobjdir)/bind.o: rts/bind.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) 

$(libgpcobjdir)/randfile.o: rts/randfile.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) 

$(libgpcobjdir)/pexecute.o: rts/pexecute.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) 

$(libgpcobjdir)/internal.o: rts/internal.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/string.o: rts/string.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/error.o: rts/error.pas rts/rts-config.inc
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/time.o: rts/time.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/random.o: rts/random.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/string2.o: rts/string2.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/filename.o: rts/filename.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/files.o: rts/files.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/getopt.o: rts/getopt.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/numtodec.o: rts/numtodec.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/sets.o: rts/sets.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/heap.o: rts/heap.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/math.o: rts/math.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/endian.o: rts/endian.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/move.o: rts/move.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)

$(libgpcobjdir)/init.o: rts/init.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) -DRTS_RELEASE_STRING=\"20000813\" $(GPC_FLAGS)

$(libgpcobjdir)/gpc.o: rts/gpc.pas
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(GPC_FLAGS)
