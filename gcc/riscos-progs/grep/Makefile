INCLUDES = -I.
CFLAGS = -O2 $(OPTS) $(TARGET_DEFS) -DHAVE_CONFIG_H -D_PTR_T
CC = $(standard_exec_prefix)/gcc $(CFLAGS)

$(objdir)/riscos-progs/grep/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/grep/btowc.o \
	$(objdir)/riscos-progs/grep/dfa.o \
	$(objdir)/riscos-progs/grep/grep.o \
	$(objdir)/riscos-progs/grep/kwset.o \
	$(objdir)/riscos-progs/grep/obstack.o \
	$(objdir)/riscos-progs/grep/regex.o \
	$(objdir)/riscos-progs/grep/savedir.o \
	$(objdir)/riscos-progs/grep/search.o

all:	$(BIN)/grep $(BIN)/egrep $(BIN)/fgrep

$(BIN)/grep:	$(OBJS) $(objdir)/riscos-progs/grep/grepmat.o $(TBIN)/$(TRG)/unixlib.o
	$(standard_exec_prefix)/gcc -o $@ $(OBJS) $(objdir)/riscos-progs/grep/grepmat.o

$(BIN)/egrep:	$(OBJS) $(objdir)/riscos-progs/grep/egrepmat.o $(TBIN)/$(TRG)/unixlib.o
	$(standard_exec_prefix)/gcc -o $@ $(OBJS) $(objdir)/riscos-progs/grep/egrepmat.o

$(BIN)/fgrep:	$(OBJS) $(objdir)/riscos-progs/grep/fgrepmat.o $(TBIN)/$(TRG)/unixlib.o
	$(standard_exec_prefix)/gcc -o $@ $(OBJS) $(objdir)/riscos-progs/grep/fgrepmat.o

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
