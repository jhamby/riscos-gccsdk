INCLUDES = -I. -I../../ -I../../include
CFLAGS = -O2 $(OPTS) $(TARGET_DEFS) -DHAVE_CONFIG_H
CC = gcc $(CFLAGS)

$(objdir)/riscos-aof/ld/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-aof/ld/ld.o \
	$(objdir)/riscos-aof/ld/rname.o

all:	$(TBIN)/ld

$(TBIN)/ld:	$(OBJS)
	gcc -o $@ $(OBJS) -L$(objdir)/libiberty -liberty

$(objdir)/riscos-aof/ld/ld.o: ld.c
	$(CC) -o $@ -c ld.c $(INCLUDES)	-DSTANDARD_EXEC_PREFIX=\"$(standard_exec_prefix)\"

$(objdir)/riscos-aof/ld/rname.o: rname.c
