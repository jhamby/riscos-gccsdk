INCLUDES = -I.
CFLAGS = -O2 $(OPTS) $(TARGET_DEFS) -DHAVE_CONFIG_H
CC = $(standard_exec_prefix)/gcc $(CFLAGS)

$(objdir)/riscos-progs/sed/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/sed/compile.o \
	$(objdir)/riscos-progs/sed/execute.o \
	$(objdir)/riscos-progs/sed/regex.o \
	$(objdir)/riscos-progs/sed/sed.o \
	$(objdir)/riscos-progs/sed/utils.o

all:	$(BIN)/sed

$(BIN)/sed:	$(OBJS) $(TBIN)/$(TRG)/unixlib.o
	$(standard_exec_prefix)/gcc -o $@ $(OBJS)

$(objdir)/riscos-progs/sed/compile.o: compile.c
$(objdir)/riscos-progs/sed/execute.o: execute.c
$(objdir)/riscos-progs/sed/regex.o: regex.c
$(objdir)/riscos-progs/sed/sed.o: sed.c
$(objdir)/riscos-progs/sed/utils.o: utils.c
