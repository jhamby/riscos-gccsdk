# Makefile for GNU make - a build management tool
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I.
CROSS_CFLAGS += -DHAVE_CONFIG_H -DLOCALEDIR=\".\" -DINCLUDEDIR=\".\" \
                -DLIBDIR=\".\"

$(objdir)/riscos-progs/make/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/make/ar.o \
	$(objdir)/riscos-progs/make/arscan.o \
	$(objdir)/riscos-progs/make/commands.o \
	$(objdir)/riscos-progs/make/default.o \
	$(objdir)/riscos-progs/make/dir.o \
	$(objdir)/riscos-progs/make/expand.o \
	$(objdir)/riscos-progs/make/file.o \
	$(objdir)/riscos-progs/make/function.o \
	$(objdir)/riscos-progs/make/getloadavg.o \
	$(objdir)/riscos-progs/make/hash.o \
	$(objdir)/riscos-progs/make/implicit.o \
	$(objdir)/riscos-progs/make/job.o \
	$(objdir)/riscos-progs/make/main.o \
	$(objdir)/riscos-progs/make/misc.o \
	$(objdir)/riscos-progs/make/read.o \
	$(objdir)/riscos-progs/make/remake.o \
	$(objdir)/riscos-progs/make/remote-stub.o \
	$(objdir)/riscos-progs/make/rule.o \
	$(objdir)/riscos-progs/make/signame.o \
	$(objdir)/riscos-progs/make/variable.o \
	$(objdir)/riscos-progs/make/version.o \
	$(objdir)/riscos-progs/make/vpath.o

all:	$(bin_dir)/make$(exesfx)

$(bin_dir)/make$(exesfx):	$(OBJS)
	$(CROSS_CC) -o $@ $(OBJS) -L$(objdir)/libiberty/$(STAGE) -liberty

$(objdir)/riscos-progs/make/ar.o: ar.c
$(objdir)/riscos-progs/make/arscan.o: arscan.c
$(objdir)/riscos-progs/make/commands.o: commands.c
$(objdir)/riscos-progs/make/default.o: default.c
$(objdir)/riscos-progs/make/dir.o: dir.c
$(objdir)/riscos-progs/make/expand.o: expand.c
$(objdir)/riscos-progs/make/file.o: file.c
$(objdir)/riscos-progs/make/function.o: function.c
$(objdir)/riscos-progs/make/getloadavg.o: getloadavg.c
$(objdir)/riscos-progs/make/gettext.o: gettext.c gettext.h
$(objdir)/riscos-progs/make/implicit.o: implicit.c
$(objdir)/riscos-progs/make/job.o: job.c
$(objdir)/riscos-progs/make/main.o: main.c
$(objdir)/riscos-progs/make/misc.o: misc.c
$(objdir)/riscos-progs/make/read.o: read.c
$(objdir)/riscos-progs/make/remake.o: remake.c
$(objdir)/riscos-progs/make/remote-stub.o: remote-stub.c
$(objdir)/riscos-progs/make/rule.o: rule.c
$(objdir)/riscos-progs/make/signame.o: signame.c
$(objdir)/riscos-progs/make/variable.o: variable.c
$(objdir)/riscos-progs/make/version.o: version.c
$(objdir)/riscos-progs/make/vpath.o: vpath.c
