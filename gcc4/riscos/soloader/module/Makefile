# Makefile for Shared Object Manager module
# By Lee Noar
# Started 30th June, 2004

include ../config.mk

OBJDIR = $(BUILDDIR)/module

$(OBJDIR)/%.o: %.s
	$(CC) -c $< -o $@

MODULE_ELF	= $(OBJDIR)/SoManager

MODULE_BIN	= $(OBJDIR)/SoManager,ffa

OBJS		= $(OBJDIR)/so-module.o

EXTRA_OBJS	= $(BUILDDIR)/utils/utils.o $(BUILDDIR)/utils/div.o

all: $(OBJDIR) $(MODULE_BIN)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	
# Don't replace the active module, this could be dangerous
$(MODULE_BIN): $(MODULE_ELF)
	$(OBJCOPY) -O binary $(MODULE_ELF) $(MODULE_BIN)

$(MODULE_ELF): $(OBJS) $(EXTRA_OBJS)
	$(CC) -o $(MODULE_ELF) $(OBJS) $(EXTRA_OBJS) -static -Ttext 0 -nostdlib -N

$(OBJDIR)/so-module.o: so-module.s so-module.h ../utils/macros.h ../utils/elf-consts.h ../utils/at-consts.h ../utils/swis.h ../rt_workspace.h
