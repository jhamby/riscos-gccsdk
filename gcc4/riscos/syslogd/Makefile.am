# SysLog Daemon
# Copyright 1999-2010 John Tytgat, Alex Waugh

bin_PROGRAMS=SysLogD
SysLogD_SOURCES=main.c modheader.cmhg
BUILT_SOURCES=modheader.h
SysLogD_CFLAGS=-O3 -mmodule -Wall -std=gnu99
SysLogD_CCASFLAGS=-O3 -mmodule -Wall -std=gnu99
SysLogD_LDFLAGS=-mmodule

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
	test -f "$(DESTDIR)$(bindir)/SysLogD$(EXEEXT)" && mv $(DESTDIR)$(bindir)/SysLogD$(EXEEXT) $(DESTDIR)$(bindir)/SysLogD$(EXEEXT)\,ffa
