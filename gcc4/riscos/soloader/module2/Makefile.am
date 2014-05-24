# Process this file with automake to generate Makefile.in
#
# Copyright 2007-2012 GCCSDK Developers
# Written by John Tytgat

bin_PROGRAMS=SOManager
SOManager_SOURCES=link_list.c som_array.c som_elf.c som_init.c som_os_swis.c \
	som_runcom.c som_symlinks.c som_alloc.c som_command.c som_final.c \
	som_main.c som_register.c som_startapp.s som_swihandler.c \
	som_utilswis.c som_utils.c som_history.c \
	somanager.cmhg
BUILT_SOURCES=somanager.h
SOManager_CFLAGS=-O3 -mmodule -Wall -std=gnu99
SOManager_CCASFLAGS=-O3 -mmodule -Wall -std=gnu99
SOManager_LDFLAGS=-mmodule

# CMunge invocation:
AM_V_CMUNGE = $(am__v_CMUNGE_$(V))
am__v_CMUNGE_ = $(am__v_CMUNGE_$(AM_DEFAULT_VERBOSITY))
am__v_CMUNGE_0 = @echo "  CMUNGE" $@;
SUFFIXES=.cmhg
CMUNGEFLAGS = -tgcc -32bit -p -DPACKAGE_VERSION=$(PACKAGE_VERSION)
.cmhg.h:
	$(AM_V_CMUNGE)@CMUNGE@ $(CMUNGEFLAGS) -d $@ $<
.cmhg.o:
	$(AM_V_CMUNGE)@CMUNGE@ $(CMUNGEFLAGS) -o $@ $<

# Add ,ffa suffix to RISC OS Module filename during installation:
install-exec-hook:
	test -f "$(DESTDIR)$(bindir)/SOManager$(EXEEXT)" && mv $(DESTDIR)$(bindir)/SOManager$(EXEEXT) $(DESTDIR)$(bindir)/SOManager$(EXEEXT)\,ffa
