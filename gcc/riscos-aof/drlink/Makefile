INCLUDES = -I.
CFLAGS = -g2 $(OPTS) $(TARGET_DEFS) -DTARGET_UNIXLIB
CC = gcc $(CFLAGS)

$(objdir)/riscos-aof/drlink/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

OBJS =	$(objdir)/riscos-aof/drlink/Aoffiles.o \
	$(objdir)/riscos-aof/drlink/Areas.o \
	$(objdir)/riscos-aof/drlink/C++names.o \
	$(objdir)/riscos-aof/drlink/Debug.o \
	$(objdir)/riscos-aof/drlink/Drlmain.o \
	$(objdir)/riscos-aof/drlink/Files.o \
	$(objdir)/riscos-aof/drlink/Heap.o \
	$(objdir)/riscos-aof/drlink/Libraries.o \
	$(objdir)/riscos-aof/drlink/Linkdebug.o \
	$(objdir)/riscos-aof/drlink/Linkedit.o \
	$(objdir)/riscos-aof/drlink/Linkfiles.o \
	$(objdir)/riscos-aof/drlink/Messages.o \
	$(objdir)/riscos-aof/drlink/Stdcode.o \
	$(objdir)/riscos-aof/drlink/Symbols.o

all:	$(BIN)/drlink

$(BIN)/drlink:	$(OBJS)
	gcc -o $@ $(OBJS)

$(objdir)/riscos-aof/drlink/Aoffiles.o: Aoffiles.c
$(objdir)/riscos-aof/drlink/Areas.o: Areas.c
$(objdir)/riscos-aof/drlink/C++names.o: C++names.c
$(objdir)/riscos-aof/drlink/Debug.o: Debug.c
$(objdir)/riscos-aof/drlink/Drlmain.o: Drlmain.c
$(objdir)/riscos-aof/drlink/Files.o: Files.c
$(objdir)/riscos-aof/drlink/Heap.o: Heap.c
$(objdir)/riscos-aof/drlink/Libraries.o: Libraries.c
$(objdir)/riscos-aof/drlink/Linkdebug.o: Linkdebug.c
$(objdir)/riscos-aof/drlink/Linkedit.o: Linkedit.c
$(objdir)/riscos-aof/drlink/Linkfiles.o: Linkfiles.c
$(objdir)/riscos-aof/drlink/Messages.o: Messages.c
$(objdir)/riscos-aof/drlink/Stdcode.o: Stdcode.c
$(objdir)/riscos-aof/drlink/Symbols.o: Symbols.c
