# GDBServer -- a RISC OS remote debug server for the GNU debugger (gdb)
# Copyright 2009 J-M Bell
# Copyright 2010 GCCSDK Developers

bin_PROGRAMS=GDBServer

OURWARNFLAGS=-Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic

GDBServer_SOURCES=debug.c gdb.c module.c session.c socket.c step.c \
	asmutils.s session_asm.s \
	header.cmhg
BUILT_SOURCES=header.h
GDBServer_CFLAGS=-O3 -mmodule $(OURWARNFLAGS) -std=gnu99 -DCONFIG_DEBUG
GDBServer_CCASFLAGS=-O3 -mmodule $(OURWARNFLAGS) -std=gnu99
GDBServer_LDFLAGS=-mmodule

# CMunge invocation:
SUFFIXES=.cmhg
CMUNGE = cmunge
CMUNGEFLAGS = -p -tgcc -32bit -apcs 3/32/nonreent/fpe2/noswst/nofpr/nofp
.cmhg.h:
	$(CMUNGE) $(CMUNGEFLAGS) -d $@ $<
.cmhg.o:
	$(CMUNGE) $(CMUNGEFLAGS) -o $@ $<

# Add ,ffa suffix to RISC OS Module filename during installation:
install-exec-hook:
	test -f "$(DESTDIR)$(bindir)/GDBServer$(EXEEXT)" && mv $(DESTDIR)$(bindir)/GDBServer$(EXEEXT) $(DESTDIR)$(bindir)/GDBServer$(EXEEXT)\,ffa
