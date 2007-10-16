# Simple C programs.
# All compiled with SharedCLibrary

CC = gcc
CFLAGS = -mlibscl -O3
LDFLAGS = -mlibscl

all: ackermann dhrystone helloworld

ACK_OBJ = ackermann.o
ackermann: $(ACK_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(ACK_OBJ)

DHRY_OBJ = dhry_1.o dhry_2.o
dhrystone: $(DHRY_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(DHRY_OBJ)

HLLW_OBJ = helloworld.o
helloworld: $(HLLW_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(HLLW_OBJ)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
