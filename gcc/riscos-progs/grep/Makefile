# Makefile for GNU grep
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I.
CROSS_CFLAGS += -DHAVE_CONFIG_H -D_PTR_T

$(objdir)/riscos-progs/grep/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/grep/btowc.o \
	$(objdir)/riscos-progs/grep/dfa.o \
	$(objdir)/riscos-progs/grep/grep.o \
	$(objdir)/riscos-progs/grep/kwset.o \
	$(objdir)/riscos-progs/grep/obstack.o \
	$(objdir)/riscos-progs/grep/regex.o \
	$(objdir)/riscos-progs/grep/savedir.o \
	$(objdir)/riscos-progs/grep/search.o

all:	$(bin_dir)/grep $(bin_dir)/egrep $(bin_dir)/fgrep

$(bin_dir)/grep:	$(OBJS) $(objdir)/riscos-progs/grep/grepmat.o
	$(CROSS_CC) -o $@ $(OBJS) $(objdir)/riscos-progs/grep/grepmat.o

$(bin_dir)/egrep:	$(OBJS) $(objdir)/riscos-progs/grep/egrepmat.o
	$(CROSS_CC) -o $@ $(OBJS) $(objdir)/riscos-progs/grep/egrepmat.o

$(bin_dir)/fgrep:	$(OBJS) $(objdir)/riscos-progs/grep/fgrepmat.o
	$(CROSS_CC) -o $@ $(OBJS) $(objdir)/riscos-progs/grep/fgrepmat.o

$(objdir)/riscos-progs/grep/grep.o:	grep.c
$(objdir)/riscos-progs/grep/btowc.o:	btowc.c
$(objdir)/riscos-progs/grep/dfa.o:	dfa.c
$(objdir)/riscos-progs/grep/egrepmat.o:	egrepmat.c
$(objdir)/riscos-progs/grep/fgrepmat.o:	fgrepmat.c
$(objdir)/riscos-progs/grep/grepmat.o:	grepmat.c
$(objdir)/riscos-progs/grep/kwset.o:	kwset.c
$(objdir)/riscos-progs/grep/obstack.o:	obstack.c
$(objdir)/riscos-progs/grep/regex.o:	regex.c
$(objdir)/riscos-progs/grep/savedir.o:	savedir.c
$(objdir)/riscos-progs/grep/search.o:	search.c
