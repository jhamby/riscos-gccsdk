# Makefile for DecAOF
#
# Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk

CFLAGS += -O2 
LIBS=
INSTALL=install -s
INSTDIR=$${HOME}/bin
BACKUPFILE=$${HOME}/tmp/decaof.tar.Z

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE -DUNIX
exesfx =
else
CC = $(CROSS_CC)
CFLAGS += $(CROSS_CFLAGS) -DRISCOS3
endif

decaofobjdir = $(objdir)/riscos-aof/decaof/$(STAGE)

$(decaofobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(decaofobjdir)/%.o: %.s
	$(CC) $(CFLAGS) -o $@ -c $<

# edit no more

OBJS=	$(decaofobjdir)/main.o $(decaofobjdir)/decode.o \
	$(decaofobjdir)/misc.o $(decaofobjdir)/io.o \
	$(decaofobjdir)/error.o
SRCS=	main.c decode.c misc.c io.c error.c

$(bin_dir)/decaof: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

depend:	$(SRCS)
	makedepend $(CFLAGS) -- $(SRCS)
	mv makefile makefile.old
	sed -f nosysdep.sed < makefile.old > makefile
	rm -f makefile.old makefile.bak

install:	decaof
	$(INSTALL) decaof $(INSTDIR)

clean:
	rm -f decaof $(OBJS) endian.h mkendian core a.out *~

backup:	clean
	(cd ..; tar cfv - decaof | compress -f > $(BACKUPFILE))

#decaof.h:	endian.h

#endian.h:	mkendian
#	mkendian > endian.h

#mkendian:	mkendian.c
#	$(CC) $(CFLAGS) -o mkendian mkendian.c

# DO NOT DELETE THIS LINE -- make depend depends on it.

