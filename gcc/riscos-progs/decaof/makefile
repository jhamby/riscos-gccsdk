# Makefile for DecAOF
#
# Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk

CC=	gcc
CFLAGS= -O2 -DRISCOS3
LIBS=
INSTALL=install -s
INSTDIR=$${HOME}/bin
BACKUPFILE=$${HOME}/tmp/decaof.tar.Z

# edit no more

OBJS=	main.o decode.o misc.o io.o error.o
SRCS=	main.c decode.c misc.c io.c error.c

decaof:	$(OBJS)
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

decaof.h:	endian.h

endian.h:	mkendian
	mkendian > endian.h

mkendian:	mkendian.c
	$(CC) $(CFLAGS) -o mkendian mkendian.c

# DO NOT DELETE THIS LINE -- make depend depends on it.

main.o: decaof.h cproto.h io.h misc.h error.h
decode.o: decaof.h cproto.h io.h main.h misc.h
decode.o: error.h 
misc.o: decaof.h
io.o: decaof.h cproto.h error.h 
error.o: error.h cproto.h 
