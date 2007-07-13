# Makefile for ln
# Copyright 2007 GCCSDK Developers
# By Lee Noar
# 10th July, 2007

include ../config.mk

HOST_OBJDIR = $(BUILDDIR)/ln/host

CROSS_OBJDIR = $(BUILDDIR)/ln/cross

CFLAGS	= -O3 -Wall

$(HOST_OBJDIR)/%.o: %.c
	$(HOST_CC) $(CFLAGS) -c $< -o $@

HOST_PROGRAM		= $(HOST_OBJDIR)/ln

HOST_OBJS		= $(HOST_OBJDIR)/ln.o

$(CROSS_OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -mpoke-function-name -DCROSS_COMPILE -fcall-used-v5 -c $< -o $@

CROSS_PROGRAM		= $(CROSS_OBJDIR)/ln,e1f

CROSS_OBJS		= $(CROSS_OBJDIR)/ln.o

all: $(HOST_OBJDIR) $(HOST_PROGRAM) $(CROSS_OBJDIR) $(CROSS_PROGRAM)

$(HOST_OBJDIR):
	mkdir -p $(HOST_OBJDIR)

$(HOST_PROGRAM): $(HOST_OBJS)
	$(HOST_CC) -o $(HOST_PROGRAM) $(HOST_OBJS)

$(CROSS_OBJDIR):
	mkdir -p $(CROSS_OBJDIR)

$(CROSS_PROGRAM): $(CROSS_OBJS)
	$(CC) -o $(CROSS_PROGRAM) $(CROSS_OBJS)

clean:
	rm -frv $(BUILDDIR)/ln
