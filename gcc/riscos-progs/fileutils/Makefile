INCLUDES = -I.
CFLAGS = -O2 $(OPTS) $(TARGET_DEFS) -DHAVE_CONFIG_H
CC = $(standard_exec_prefix)/gcc $(CFLAGS)

$(objdir)/riscos-progs/fileutils/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

TOUCH_OBJS = $(objdir)/riscos-progs/fileutils/touch.o \
	$(objdir)/riscos-progs/fileutils/error.o \
	$(objdir)/riscos-progs/fileutils/argmatch.o \
	$(objdir)/riscos-progs/fileutils/getdate.o \
	$(objdir)/riscos-progs/fileutils/posixtm.o \
	$(objdir)/riscos-progs/fileutils/safe-read.o \
	$(objdir)/riscos-progs/fileutils/closeout.o

MKDIR_OBJS = $(objdir)/riscos-progs/fileutils/mkdir.o \
	$(objdir)/riscos-progs/fileutils/error.o \
	$(objdir)/riscos-progs/fileutils/modechange.o \
	$(objdir)/riscos-progs/fileutils/makepath.o \
	$(objdir)/riscos-progs/fileutils/save-cwd.o \
	$(objdir)/riscos-progs/fileutils/xgetcwd.o \
	$(objdir)/riscos-progs/fileutils/stripslash.o \
	$(objdir)/riscos-progs/fileutils/xmalloc.o \
	$(objdir)/riscos-progs/fileutils/closeout.o

all:	$(BIN)/touch $(BIN)/mkdir

$(BIN)/touch:	$(TOUCH_OBJS) $(TBIN)/$(TRG)/unixlib.o $(TBIN)/$(TRG)/libiberty.o
	$(standard_exec_prefix)/gcc -o $@ $(TOUCH_OBJS) $(TBIN)/$(TRG)/libiberty.o

$(BIN)/mkdir:	$(MKDIR_OBJS) $(TBIN)/$(TRG)/unixlib.o $(TBIN)/$(TRG)/libiberty.o
	$(standard_exec_prefix)/gcc -o $@ $(MKDIR_OBJS) $(TBIN)/$(TRG)/libiberty.o

$(objdir)/riscos-progs/fileutils/touch.o: touch.c
$(objdir)/riscos-progs/fileutils/error.o: error.c
$(objdir)/riscos-progs/fileutils/argmatch.o: argmatch.c
$(objdir)/riscos-progs/fileutils/getdate.o: getdate.c
$(objdir)/riscos-progs/fileutils/posixtm.o: posixtm.c
$(objdir)/riscos-progs/fileutils/safe-read.o: safe-read.c
$(objdir)/riscos-progs/fileutils/closeout.o: closeout.c
$(objdir)/riscos-progs/fileutils/mkdir.o: mkdir.c
$(objdir)/riscos-progs/fileutils/modechange.o: modechange.c
$(objdir)/riscos-progs/fileutils/makepath.o: makepath.c
$(objdir)/riscos-progs/fileutils/save-cwd.o: save-cwd.c
$(objdir)/riscos-progs/fileutils/xgetcwd.o: xgetcwd.c
$(objdir)/riscos-progs/fileutils/stripslash.o: stripslash.c
$(objdir)/riscos-progs/fileutils/xmalloc.o: xmalloc.c
