# Makefile for Ben Summers' run time linker - rink
# Written by Nick Burrett <nick@dsvr.net>

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE
exesfx = 
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS) -DUNIXLIB
endif

INCLUDES = -I. -I../../
CFLAGS += -Wall

rinkobjdir = $(objdir)/riscos-aof/rink/$(STAGE)

$(rinkobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(rinkobjdir)/%.o: %.s
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

EXTRACTSYM = $(rinkobjdir)/extractsym.o

all:	$(bin_dir)/rink$(exesfx) $(bin_dir)/extractsym$(exesfx) \
	$(bin_dir)/rinkptr$(exesfx)

$(bin_dir)/extractsym$(exesfx):	$(rinkobjdir)/extractsym.o
	$(CC) -o $@ $(rinkobjdir)/extractsym.o

$(bin_dir)/test$(exesfx): 	$(rinkobjdir)/test.o $(rinkobjdir)/rink_rtsys.o \
	$(rinkobjdir)/pointerblk.o
	$(CC) -o $@ $(rinkobjdir)/test.o $(rinkobdir)/rink_rtsys.o \
	$(rinkobjdir)/pointerblk.o

$(bin_dir)/rink$(exesfx):	$(rinkobjdir)/rink.o
	$(CC) -o $@ $(rinkobjdir)/rink.o

$(bin_dir)/rinkptr$(exesfx):	$(rinkobjdir)/rinkptr.o
	$(CC) -o $@ $(rinkobjdir)/rinkptr.o

$(rinkobjdir)/extractsym.o: extractsym.c
$(rinkobjdir)/pointerblk.o: pointerblk.c
$(rinkobjdir)/rink.o: rink.c
$(rinkobjdir)/rink_ptr.o: rink_ptr.c
$(rinkobjdir)/rink_rtsys.o: rink_rtsys.c
$(rinkobjdir)/test.o: test.c
