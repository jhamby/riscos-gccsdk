# Process this file with automake to generate Makefile.in
#
# Copyright 2007 GCCSDK Developers
# Written by John Tytgat

bin_PROGRAMS=SOManager
SOManager_SOURCES=link_list.c som_array.c som_elf.c som_init.c som_os_swis.c \
	som_runcom.c som_symlinks.c som_alloc.c som_command.c som_final.c \
	som_main.c som_register.c som_startapp.s som_swihandler.c \
	som_utilswis.c \
	somanager.cmhg
BUILT_SOURCES=somanager.h
SOManager_CFLAGS=-O3 -mmodule
SOManager_CCASFLAGS=-O3 -mmodule

# CMunge invocation:
SUFFIXES=.cmhg
CMUNGE = cmunge
CMUNGEFLAGS = -tgcc -32bit -p
.cmhg.h:
	$(CMUNGE) $(CMUNGEFLAGS) -d $@ $<
.cmhg.o:
	$(CMUNGE) $(CMUNGEFLAGS) -o $@ $<

# Add ,ffa suffix to RISC OS Module filename during installation:
install-exec-hook:
	test -f "$(DESTDIR)$(bindir)/SOManager$(EXEEXT)" && mv $(DESTDIR)$(bindir)/SOManager$(EXEEXT) $(DESTDIR)$(bindir)/SOManager$(EXEEXT)\,ffa
