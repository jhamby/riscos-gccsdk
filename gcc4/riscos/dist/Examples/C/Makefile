# Simple C programs which are compiled 3 times:
#   1) using SharedCLibrary as runtime library
#   2) using static UnixLib as runtime library
#   3) using shared UnixLib as runtime library

CC = gcc
# Compile + link flags for SharedCLibrary as runtime library:
CFLAGS_SCL = -mlibscl -O3
# Compile + link flags for static UnixLib as runtime library. Note that
# '-static' option is a linker option and should not be specified when
# only compiling.  The '-Wl,-s' option instructs the linker to strip
# the resulting ELF binary resulting in smaller binaries (not without
# runtime difference as the ELF loader will not load the symbol and
# debugging information).
CFLAGS_ULST = -static -Wl,-s -O3
# Compile + link flags for shared UnixLib as runtime library (default):
CFLAGS_ULSO = -Wl,-s -O3

all: ackermann_scl dhrystone_scl helloworld_scl \
	ackermann_static_ul dhrystone_static_ul helloworld_static_ul \
	ackermann_shared_ul dhrystone_shared_ul helloworld_shared_ul

ackermann_scl: ackermann.c
	$(CC) $(CFLAGS_SCL) -o $@ ackermann.c

ackermann_static_ul: ackermann.c
	$(CC) $(CFLAGS_ULST) -o $@ ackermann.c

ackermann_shared_ul: ackermann.c
	$(CC) $(CFLAGS_ULSO) -o $@ ackermann.c

dhrystone_scl: dhry_1.c dhry_2.c
	$(CC) $(CFLAGS_SCL) -o $@ dhry_1.c dhry_2.c

dhrystone_static_ul: dhry_1.c dhry_2.c
	$(CC) $(CFLAGS_ULST) -o $@ dhry_1.c dhry_2.c

dhrystone_shared_ul: dhry_1.c dhry_2.c
	$(CC) $(CFLAGS_ULSO) -o $@ dhry_1.c dhry_2.c

helloworld_scl: helloworld.c
	$(CC) $(CFLAGS_SCL) -o $@ helloworld.c

helloworld_static_ul: helloworld.c
	$(CC) $(CFLAGS_ULST) -o $@ helloworld.c

helloworld_shared_ul: helloworld.c
	$(CC) $(CFLAGS_ULSO) -o $@ helloworld.c
