INCLUDES = -I./
CFLAGS = -O2 $(OPTS) $(TARGET_DEFS) -DHAVE_CONFIG_H
CC = $(standard_exec_prefix)/gcc $(CFLAGS)

$(objdir)/riscos-progs/patch/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/patch/addext.o \
	$(objdir)/riscos-progs/patch/argmatch.o \
	$(objdir)/riscos-progs/patch/backupfile.o \
	$(objdir)/riscos-progs/patch/basename.o \
	$(objdir)/riscos-progs/patch/getopt.o \
	$(objdir)/riscos-progs/patch/getopt1.o \
	$(objdir)/riscos-progs/patch/inp.o \
	$(objdir)/riscos-progs/patch/maketime.o \
	$(objdir)/riscos-progs/patch/partime.o \
	$(objdir)/riscos-progs/patch/patch.o \
	$(objdir)/riscos-progs/patch/pch.o \
	$(objdir)/riscos-progs/patch/quotearg.o \
	$(objdir)/riscos-progs/patch/util.o \
	$(objdir)/riscos-progs/patch/version.o

all:	$(BIN)/patch

$(BIN)/patch:	$(OBJS) $(TBIN)/$(TRG)/unixlib.o
	$(standard_exec_prefix)/gcc -o $@ $(OBJS)

$(objdir)/riscos-progs/patch/addext.o: addext.c
$(objdir)/riscos-progs/patch/argmatch.o: argmatch.c
$(objdir)/riscos-progs/patch/backupfile.o: backupfile.c
$(objdir)/riscos-progs/patch/basename.o: basename.c
$(objdir)/riscos-progs/patch/getopt.o: getopt.c
$(objdir)/riscos-progs/patch/getopt1.o: getopt1.c
$(objdir)/riscos-progs/patch/inp.o: inp.c
$(objdir)/riscos-progs/patch/maketime.o: maketime.c
$(objdir)/riscos-progs/patch/partime.o: partime.c
$(objdir)/riscos-progs/patch/patch.o: patch.c
$(objdir)/riscos-progs/patch/pch.o: pch.c
$(objdir)/riscos-progs/patch/quotearg.o: quotearg.c
$(objdir)/riscos-progs/patch/util.o: util.c
$(objdir)/riscos-progs/patch/version.o: version.c
