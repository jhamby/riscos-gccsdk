# Makefile for runtime library

include ../config.mk

OBJDIR = $(BUILDDIR)/rt_lib

$(OBJDIR)/%.o: %.s
	$(CC) -fPIC -c $< -o $@

OBJS = $(OBJDIR)/_rt_loadpic.o

LIB = $(OBJDIR)/libpic.a

all: $(OBJDIR) $(LIB)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(LIB): $(OBJS)
	$(AR) -cr $(LIB) $(OBJS)

$(OBJDIR)/_rt_loadpic.o: _rt_loadpic.s ../rt_workspace.h
