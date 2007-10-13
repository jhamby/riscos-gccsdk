# Simple C programs.
# All compiled with SharedCLibrary

CFLAGS = -mlibscl -O3
LDFLAGS = -mlibscl

all: ackermann dhrystone helloworld

ACK_OBJ = ackermann.o
ackermann: $(ACK_OBJ)
	$(LINK) $(LDFLAGS) -o $@ $(ACK_OBJ)

DHRY_OBJ = dhry_1.o dhry_2.o
dhrystone: $(DHRY_OBJ)
	$(LINK) $(LDFLAGS) -o $@ $(DHRY_OBJ)

HLLW_OBJ = helloworld.o
helloworld: $(HLLW_OBJ)
	$(LINK) $(LDFLAGS) -o $@ $(HLLW_OBJ)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
