CC = arm-unknown-riscos-gcc
CFLAGS = -O3 -mmodule
# Add -Wl,-M to get the linker map.
LINKFLAGS = -mmodule
CMHG = cmunge
CMHGFLAGS = -tgcc -32bit

COBJS = check.c def.c

OBJS = $(COBJS:.c=.o) MdlHdr.o

TARGET = testgccmodule,ffa

.PHONY: all clean

all: $(TARGET)

clean:
	-rm $(OBJS) MdlHdr.h $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LINKFLAGS) $(OBJS) -o $@

$(OBJS): MdlHdr.h

.SUFFIXES:
.SUFFIXES: .c .s .o .h .cmhg

.c.o:;	$(CC) $(CFLAGS) -c -o $@ $<
.s.o:;	$(CC) $(CFLAGS) -c -o $@ $<

# CMunge invocation rule:
.cmhg.h:;	$(CMHG) $(CMHGFLAGS) -d $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

