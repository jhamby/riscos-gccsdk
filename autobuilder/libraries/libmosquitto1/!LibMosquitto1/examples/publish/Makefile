HEADERS =
OBJECTS = basic-1.o

WARNFLAGS = -Wall

CC = gcc
CFLAGS = -O3 -ILibMosquitto1:
LDFLAGS = -static -LLibMosquitto1: -lmosquitto_static -L<LibSSL1.1$$Dir> -lssl -lcrypto
	
basic-1: $(OBJECTS)
	$(CC) -o $@ $(LDFLAGS) $^
	@elf2aif $@

%.o : %.c $(HEADERS)
	@echo "==> $<"
	@$(CC) -o $@ -c $(CFLAGS) $<

clean:
	-rm -f $(OBJECTS) basic-1
