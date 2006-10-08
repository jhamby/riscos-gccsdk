# Makefile for ELF loader utilities

include ../config.mk

OBJDIR = $(BUILDDIR)/utils

$(OBJDIR)/%.o: %.s
	$(CC) -c $< -o $@

OBJS = $(OBJDIR)/utils.o $(OBJDIR)/links.o $(OBJDIR)/div.o

all: $(OBJDIR) $(OBJS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/utils.o: utils.s elf-consts.h swis.h
$(OBJDIR)/links.o: links.s swis.h
