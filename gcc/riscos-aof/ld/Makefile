INCLUDES = -I. -I../../ -I../../include
CFLAGS += $(TARGET_DEFS) -DHAVE_CONFIG_H

$(objdir)/riscos-aof/ld/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-aof/ld/ld.o \
	$(objdir)/riscos-aof/ld/rname.o

all:	$(TBIN)/ld

$(TBIN)/ld:	$(OBJS)
	$(CC) -o $@ $(OBJS) -L$(objdir)/libiberty -liberty

$(objdir)/riscos-aof/ld/ld.o: ld.c
	$(CC) $(CFLAGS) -o $@ -c ld.c $(INCLUDES) -DSTANDARD_EXEC_PREFIX=\"$(standard_exec_prefix)\"

$(objdir)/riscos-aof/ld/rname.o: rname.c
