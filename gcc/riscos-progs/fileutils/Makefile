# Makefile for GNU fileutils
# Written by Nick Burrett <nick@sqrt.co.uk>

INCLUDES = -I.
CROSS_CFLAGS += -DHAVE_CONFIG_H

TOUCH_SRCS = touch.c error.c argmatch.c getdate.c posixtm.c \
	safe-read.c closeout.c

MKDIR_SRCS = mkdir.c error.c modechange.c makepath.c save-cwd.c \
	xgetcwd.c stripslash.c xmalloc.c closeout.c

HDRS = sys2.h

all:	$(bin_dir)/touch$(exesfx) $(bin_dir)/mkdir$(exesfx)

$(objdir)/riscos-progs/touch.o:  $(TOUCH_SRCS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(TOUCH_SRCS) $(INCLUDES)

$(objdir)/riscos-progs/mkdir.o:  $(MKDIR_SRCS) $(HDRS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(MKDIR_SRCS) $(INCLUDES)

$(bin_dir)/touch$(exesfx):	$(objdir)/riscos-progs/touch.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/touch.o $(LIBIBERTY)

$(bin_dir)/mkdir$(exesfx):	$(objdir)/riscos-progs/mkdir.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/mkdir.o $(LIBIBERTY)
