# Makefile for Shared Object Manager module v2
# Copyright 2006, 2007 GCCSDK Developers

include ../config.mk

OBJDIR = $(BUILDDIR)/module2

MODULE_BIN = $(OBJDIR)/SoManager,ffa

CFLAGS = -O3 -mmodule -Wall

LDFLAGS = -static -mmodule

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

CMUNGE = cmunge
CMUNGEFLAGS = -tgcc -32bit -p

OBJS = \
	$(OBJDIR)/som_init.o	\
	$(OBJDIR)/som_final.o	\
	$(OBJDIR)/som_command.o	\
	$(OBJDIR)/som_swihandler.o\
	$(OBJDIR)/som_main.o	\
	$(OBJDIR)/som_os_swis.o	\
	$(OBJDIR)/som_alloc.o	\
	$(OBJDIR)/som_register.o	\
	$(OBJDIR)/link_list.o	\
	$(OBJDIR)/som_utilswis.o	\
	$(OBJDIR)/som_symlinks.o	\
	$(OBJDIR)/som_elf.o	\
	$(OBJDIR)/som_runcom.o	\
	$(OBJDIR)/som_startapp.o	\
	$(OBJDIR)/som_array.o	\
	$(OBJDIR)/somanager.o

all: $(OBJDIR) $(MODULE_BIN)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	-rm -frv $(OBJDIR)
	-rm somanager.h

$(MODULE_BIN): $(OBJS) somanager.h $(OBJDIR)/somanager.o
	$(CC) $(LDFLAGS) -o SOManager,e1f $(OBJS)
	$(OBJCOPY) -O binary SOManager,e1f $@
	rm SOManager,e1f

# CMunge invocation:
somanager.h $(OBJDIR)/somanager.o: somanager.cmhg
	$(CMUNGE) $(CMUNGEFLAGS) somanager.cmhg -o $(OBJDIR)/somanager.o -d somanager.h

$(OBJDIR)/som_init.o: som_init.c somanager.h som_os_swis.h som_alloc.h som_array.h
$(OBJDIR)/som_final.o: som_final.c somanager.h som.h som_os_swis.h som_alloc.h
$(OBJDIR)/som_command.o: som_command.c somanager.h som.h som_alloc.h
$(OBJDIR)/som_swihandler.o: som_swihandler.c somanager.h som.h som_register.h som_alloc.h som_utilswis.h som_symlinks.h som_array.h
$(OBJDIR)/som_main.o: som_main.c som.h link_list.h som_alloc.h
$(OBJDIR)/som_os_swis.o: som_os_swis.c som_os_swis.h
$(OBJDIR)/som_alloc.o: som_alloc.c som_alloc.h som.h
$(OBJDIR)/som_register.o: som_register.c som_register.h som.h som_alloc.h som_workspace.h som_array.h
$(OBJDIR)/link_list.o: link_list.c link_list.h
$(OBJDIR)/som_utilswis.o: som_utilswis.c som_utilswis.h som.h somanager.h
$(OBJDIR)/som_symlinks.o: som_symlinks.c som_alloc.h somanager.h
$(OBJDIR)/som_elf.o: som_elf.c som_elf.h somanager.h som_alloc.h som_symlinks.h
$(OBJDIR)/som_runcom.o: som_alloc.h som_os_swis.h som_utilswis.h som_elf.h som.h som_register.h som_startapp.h
$(OBJDIR)/som_startapp.o: som_startapp.s
$(OBJDIR)/som_array.o: som_array.c som_array.h som.h som_alloc.h som_workspace.h
