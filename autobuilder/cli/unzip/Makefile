# Makefile for UnZip, UnZipSFX, ZipInfo and fUnZip (5.53 or later)
# using gcc 2.95.4 (or later).
# You may look at <URL:http://hard-mofo.dsvr.net/gcc/> for an up-to-date
# gcc port.

CC   = gcc
BIND = $(CC)
AS   = $(CC) -c
ASM  = $(CC) -mhard-float -xassembler-with-cpp -c
SQUEEZE = squeeze -v
E    = 

# flags
#   CFLAGS    flags for C compile
#   LFLAGS1   flags after output file spec, before obj file list
#   LFLAGS2   flags after obj file list (libraries, etc)
#
LIB      =
CFLAGS   = -O2 -mthrowback -DNO_STRNICMP -I. -mlibscl
ASMFLAGS = -mthrowback
LFLAGS1  = -mlibscl
LFLAGS2  = $(LIB)
SFXFLAGS = -DSFX -DSFX_EXDIR
FUNZIPFLAGS = -DFUNZIP

# object file lists
OBJS1 = unzip.o crc32.o crypt.o envargs.o explode.o
OBJS2 = extract.o fileio.o globals.o inflate.o list.o match.o
OBJS3 = process.o ttyio.o unreduce.o unshrink.o zipinfo.o
OBJS  = $(OBJS1) $(OBJS2) $(OBJS3) riscos.o swiven.o acorn.o
OBJF  = funzip.o crc32.o cryptf.o globalsf.o inflatef.o ttyiof.o \
	riscos.o swiven.o
OBJX  = unzipsfx.o crc32.o  crypt_.o extract_.o fileio_.o globals_.o \
	inflate_.o match_.o process_.o ttyio_.o acorn_.o swiven.o riscos_.o

UNZIP_H = unzip.h unzpriv.h globals.h riscos.h swiven.h

all:	  unzip funzip unzipsfx


VPATH = .:acorn

# suffix rules
.SUFFIXES:
.SUFFIXES: _.o f.o .o .c .s
.c_.o:
	$(CC) -c $(CFLAGS) -DUTIL ${SFXFLAGS} -o $@ $<

.cf.o:
	$(CC) -c $(CFLAGS) -DUTIL ${FUNZIPFLAGS} -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $<

.s.o:
	$(ASM) $(ASMFLAGS) -o $@ $<

unzipsfx.o: unzip.c
	$(CC) -c $(CFLAGS) $(SFXFLAGS) -o $@ $<



install:  unzip funzip unzipsfx
	$(SQUEEZE) unzip %.unzip
	$(SQUEEZE) funzip %.funzip
	$(SQUEEZE) unzipsfx unzipsfx
	copy %.unzip %.zipinfo ~CVF

# rules for unzip and funzip

unzip:	  $(OBJS)
	  $(BIND) -o unzip$(E) $(LFLAGS1) $(OBJS) $(LFLAGS2)
funzip:	  $(OBJF)
	  $(BIND) -o funzip$(E) $(LFLAGS1) $(OBJF) $(LFLAGS2)
unzipsfx: $(OBJX)
	  $(BIND) -o unzipsfx$(E) $(LFLAGS1) $(OBJX) $(LFLAGS2)

clean:
	rm -f *.o
	rm -f *ff8 *e1f

# end of Makefile
