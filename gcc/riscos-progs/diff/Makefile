INCLUDES=-I.
CFLAGS=-O2 $(OPTS) $(TARGET_DEFS) -DHAVE_CONFIG_H -D_PTR_T
CC=$(standard_exec_prefix)/gcc $(CFLAGS)

$(objdir)/riscos-progs/diff/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

DIFFOBJS = $(objdir)/riscos-progs/diff/diff.o \
	$(objdir)/riscos-progs/diff/analyze.o \
	$(objdir)/riscos-progs/diff/cmpbuf.o \
	$(objdir)/riscos-progs/diff/dir.o \
	$(objdir)/riscos-progs/diff/io.o \
	$(objdir)/riscos-progs/diff/util.o \
	$(objdir)/riscos-progs/diff/context.o \
	$(objdir)/riscos-progs/diff/ed.o \
	$(objdir)/riscos-progs/diff/ifdef.o \
	$(objdir)/riscos-progs/diff/normal.o \
	$(objdir)/riscos-progs/diff/side.o \
	$(objdir)/riscos-progs/diff/fnmatch.o \
	$(objdir)/riscos-progs/diff/getopt.o \
	$(objdir)/riscos-progs/diff/getopt1.o \
	$(objdir)/riscos-progs/diff/version.o \
	$(objdir)/riscos-progs/diff/regex.o

DIFF3OBJS = $(objdir)/riscos-progs/diff/diff3.o \
	$(objdir)/riscos-progs/diff/getopt.o \
	$(objdir)/riscos-progs/diff/getopt1.o \
	$(objdir)/riscos-progs/diff/version.o


SDIFFOBJS = $(objdir)/riscos-progs/diff/sdiff.o \
	$(objdir)/riscos-progs/diff/getopt.o \
	$(objdir)/riscos-progs/diff/getopt1.o \
	$(objdir)/riscos-progs/diff/version.o

CMPOBJS = $(objdir)/riscos-progs/diff/cmp.o \
	$(objdir)/riscos-progs/diff/cmpbuf.o \
	$(objdir)/riscos-progs/diff/error.o \
	$(objdir)/riscos-progs/diff/getopt.o \
	$(objdir)/riscos-progs/diff/getopt1.o \
	$(objdir)/riscos-progs/diff/xmalloc.o \
	$(objdir)/riscos-progs/diff/verson.o


diff3_o = diff3.o getopt.o getopt1.o version.o

diff3: $(diff3_o)
	$(link) $(diff3_o) $(linklib)
	Squeeze $@

sdiff_o = sdiff.o getopt.o getopt1.o version.o

sdiff: $(sdiff_o)
	$(link) $(sdiff_o) $(linklib)
	Squeeze $@

cmp_o = cmp.o cmpbuf.o error.o getopt.o getopt1.o \
        xmalloc.o version.o

cmp:	$(cmp_o)
	$(link) $(cmp_o) $(linklib)
	Squeeze $@


$(objdir)/riscos_progs/diff/analyze.o:	diff.h cmpbuf.h analyze.c
$(objdir)/riscos_progs/diff/cmp.o:	system.h getopt.h cmpbuf.h cmp.c
$(objdir)/riscos_progs/diff/cmpbuf.o:	system.h cmpbuf.h cmpbuf.c
$(objdir)/riscos_progs/diff/context.o:	diff.h context.c
$(objdir)/riscos_progs/diff/diff.o:		diff.h getopt.h fnmatch.h diff.c
$(objdir)/riscos_progs/diff/diff3.o:	diff.h getopt.h diff3.c
$(objdir)/riscos_progs/diff/dir.o:		diff.h dir.c
$(objdir)/riscos_progs/diff/ed.o:		diff.h ed.c
$(objdir)/riscos_progs/diff/error.o:	error.c
$(objdir)/riscos_progs/diff/fnmatch.o:	fnmatch.c
$(objdir)/riscos_progs/diff/getopt.o:	getopt.h getopt.c
$(objdir)/riscos_progs/diff/getopt1.o:	getopt.h getopt1.c
$(objdir)/riscos_progs/diff/ifdef.o:	diff.h ifdef.c
$(objdir)/riscos_progs/diff/io.o:		diff.h io.c
$(objdir)/riscos_progs/diff/memchr.o:	memchr.c
$(objdir)/riscos_progs/diff/normal.o:	diff.h normal.c
$(objdir)/riscos_progs/diff/regex.o:	regex.h regex.c
$(objdir)/riscos_progs/diff/sdiff.o:	system.h getopt.h sdiff.c
$(objdir)/riscos_progs/diff/side.o:		diff.h side.c
$(objdir)/riscos_progs/diff/util.o:		diff.h util.c
$(objdir)/riscos_progs/diff/version.o:	version.c
$(objdir)/riscos_progs/diff/waitpid.o:	system.h
$(objdir)/riscos_progs/diff/xmalloc.o:	xmalloc.c
