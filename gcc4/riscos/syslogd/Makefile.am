# SysLog Daemon
# Copyright 1999-2014 John Tytgat, Alex Waugh

bin_PROGRAMS=SysLogD
SysLogD_SOURCES=main.c modheader.cmhg
BUILT_SOURCES=modheader.h
SysLogD_CFLAGS=-O3 -mmodule -Wall -std=gnu99
SysLogD_CCASFLAGS=-O3 -mmodule -Wall -std=gnu99
SysLogD_LDFLAGS=-mmodule

# CMunge invocation:
AM_V_CMUNGE = $(am__v_CMUNGE_$(V))
am__v_CMUNGE_ = $(am__v_CMUNGE_$(AM_DEFAULT_VERBOSITY))
am__v_CMUNGE_0 = @echo "  CMUNGE" $@;
SUFFIXES=.cmhg
CMUNGEFLAGS = -tgcc -32bit -p
.cmhg.h:
	$(AM_V_CMUNGE)@CMUNGE@ $(CMUNGEFLAGS) -d $@ $<
.cmhg.o:
	$(AM_V_CMUNGE)@CMUNGE@ $(CMUNGEFLAGS) -o $@ $<

# Add ,ffa suffix to RISC OS Module filename during installation:
install-exec-hook:
	test -f "$(DESTDIR)$(bindir)/SysLogD$(EXEEXT)" && mv $(DESTDIR)$(bindir)/SysLogD$(EXEEXT) $(DESTDIR)$(bindir)/SysLogD$(EXEEXT)\,ffa
