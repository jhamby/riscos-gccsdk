# Makefile for the ARM simulator
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES=-I.
CFLAGS=-O2 -g -DP_LINUX -fomit-frame-pointer -I. -DHAVE_CONFIG_H -DDEBUG

$(objdir)/riscos-progs/arm-simulator/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/arm-simulator/armemu.o \
	$(objdir)/riscos-progs/arm-simulator/arm-dis.o \
	$(objdir)/riscos-progs/arm-simulator/armdepen.o \
	$(objdir)/riscos-progs/arm-simulator/armswi.o \
	$(objdir)/riscos-progs/arm-simulator/armdebug.o

all:	$(bin_dir)/armemu

$(bin_dir)/armemu: $(OBJS)
	$(CC) -o $@ $(OBJS)

$(objdir)/riscos-progs/arm-simulator/armemu.o: armhdr.h armfpe.h
$(objdir)/riscos-progs/arm-simulator/armswi.o: armhdr.h armswis.h
$(objdir)/riscos-progs/arm-simulator/armdisas.o: armhdr.h
$(objdir)/riscos-progs/arm-simulator/armdepen.o: armhdr.h
$(objdir)/riscos-progs/arm-simulator/armdebug.o: armhdr.h
