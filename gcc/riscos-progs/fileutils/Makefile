# Makefile for GNU fileutils
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I.
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/fileutils/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

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

all:	$(bin_dir)/touch $(bin_dir)/mkdir

$(bin_dir)/touch:	$(TOUCH_OBJS)
	$(CROSS_CC) -o $@ $(TOUCH_OBJS) $(LIBIBERTY)

$(bin_dir)/mkdir:	$(MKDIR_OBJS)
	$(CROSS_CC) -o $@ $(MKDIR_OBJS) $(LIBIBERTY)

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
