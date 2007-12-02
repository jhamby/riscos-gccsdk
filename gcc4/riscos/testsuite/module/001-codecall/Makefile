CC = arm-unknown-riscos-gcc
CFLAGS = -O3 -mmodule
CMHG = cmunge
CMHGFLAGS = -tgcc -32bit

COBJS = codecall.c codecall_fnc.c

OBJS = $(COBJS:.c=.o) MdlHdr.o

TARGET = testgccmodule,ffa

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJS): MdlHdr.h

.SUFFIXES:
.SUFFIXES: .c .o .h .cmhg

.c.o:;	$(CC) $(CFLAGS) -c -o $@ $<

# CMunge invocation rule:
.cmhg.h:;	$(CMHG) $(CMHGFLAGS) -d $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

clean:
	-rm $(OBJS) MdlHdr.h $(TARGET)
