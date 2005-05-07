# Makefile for DecAOF

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE
exesfx =
else
CC = $(CROSS_CC)
CFLAGS += $(CROSS_CFLAGS)
endif

INCLUDES = -I. -I ../../
CFLAGS += -Wall

decaofobjdir = $(objdir)/riscos-aof/decaof/$(STAGE)

all:	$(bin_dir)/decaof

$(decaofobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

SRCS = main.c decode.c misc.c io.c error.c
OBJS = $(addprefix $(decaofobjdir)/, $(SRCS:.c=.o))

$(bin_dir)/decaof: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)
