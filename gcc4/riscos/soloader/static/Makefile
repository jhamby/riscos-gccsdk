# Makefile for ELF static program loader
# By Lee Noar
# 30th June, 2004

include ../config.mk

OBJDIR = $(BUILDDIR)/static

$(OBJDIR)/%.o: %.s
	$(CC) -c $< -o $@

LOADER_ELF	= $(OBJDIR)/loader

LOADER_BIN	= $(OBJDIR)/loader,ff8

OBJS		= $(OBJDIR)/main.o $(OBJDIR)/error.o $(OBJDIR)/elf.o

EXTRA_OBJS	= $(BUILDDIR)/utils/utils.o $(BUILDDIR)/utils/links.o

all: $(OBJDIR) $(LOADER_BIN)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	
$(LOADER_BIN): $(LOADER_ELF)
	$(OBJCOPY) -O binary $(LOADER_ELF) $(LOADER_BIN)

$(LOADER_ELF): $(OBJS) $(EXTRA_OBJS)
	$(CC) -nostdlib -o $(LOADER_ELF) $(OBJS) $(EXTRA_OBJS) -static -Wl,-e -Wl,_start

$(OBJDIR)/main.o: main.s main.h ../utils/macros.h ../utils/elf-consts.h ../utils/at-consts.h ../utils/swis.h
$(OBJDIR)/error.o: error.s ../utils/swis.h
