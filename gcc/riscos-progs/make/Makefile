INCLUDES = -I./
CFLAGS = -O2 $(OPTS) $(TARGET_DEFS) -DHAVE_CONFIG_H
CC = $(standard_exec_prefix)/gcc $(CFLAGS)

$(objdir)/riscos-progs/make/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/make/ar.o \
	$(objdir)/riscos-progs/make/arscan.o \
	$(objdir)/riscos-progs/make/commands.o \
	$(objdir)/riscos-progs/make/default.o \
	$(objdir)/riscos-progs/make/dir.o \
	$(objdir)/riscos-progs/make/expand.o \
	$(objdir)/riscos-progs/make/file.o \
	$(objdir)/riscos-progs/make/function.o \
	$(objdir)/riscos-progs/make/getloadavg.o \
	$(objdir)/riscos-progs/make/implicit.o \
	$(objdir)/riscos-progs/make/job.o \
	$(objdir)/riscos-progs/make/main.o \
	$(objdir)/riscos-progs/make/misc.o \
	$(objdir)/riscos-progs/make/read.o \
	$(objdir)/riscos-progs/make/remake.o \
	$(objdir)/riscos-progs/make/remote-stu.o \
	$(objdir)/riscos-progs/make/rule.o \
	$(objdir)/riscos-progs/make/signame.o \
	$(objdir)/riscos-progs/make/variable.o \
	$(objdir)/riscos-progs/make/version.o \
	$(objdir)/riscos-progs/make/vpath.o

all:	$(BIN)/make

$(BIN)/make:	$(OBJS) $(TBIN)/$(TRG)/unixlib.o 
	$(standard_exec_prefix)/gcc -o $@ $(OBJS)

$(objdir)/riscos-progs/make/ar.o: ar.c
$(objdir)/riscos-progs/make/arscan.o: arscan.c
$(objdir)/riscos-progs/make/commands.o: commands.c
$(objdir)/riscos-progs/make/default.o: default.c
$(objdir)/riscos-progs/make/dir.o: dir.c
$(objdir)/riscos-progs/make/expand.o: expand.c
$(objdir)/riscos-progs/make/file.o: file.c
$(objdir)/riscos-progs/make/function.o: function.c
$(objdir)/riscos-progs/make/getloadavg.o: getloadavg.c
$(objdir)/riscos-progs/make/implicit.o: implicit.c
$(objdir)/riscos-progs/make/job.o: job.c
$(objdir)/riscos-progs/make/main.o: main.c
$(objdir)/riscos-progs/make/misc.o: misc.c
$(objdir)/riscos-progs/make/read.o: read.c
$(objdir)/riscos-progs/make/remake.o: remake.c
$(objdir)/riscos-progs/make/remote-stu.o: remote-stu.c
$(objdir)/riscos-progs/make/rule.o: rule.c
$(objdir)/riscos-progs/make/signame.o: signame.c
$(objdir)/riscos-progs/make/variable.o: variable.c
$(objdir)/riscos-progs/make/version.o: version.c
$(objdir)/riscos-progs/make/vpath.o: vpath.c
