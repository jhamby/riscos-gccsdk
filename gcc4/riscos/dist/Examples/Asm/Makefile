# Simple assembler + C example.  Once with GAS syntax, once with
# AsAsm syntax.

CC := gcc
CFLAGS := -mlibscl -O3
ASASM := asasm
LINKFLAGS := -mlibscl

all: asm_example

asm_example: main.o asm_gas.o asm_asasm.o
	$(CC) $(LINKFLAGS) -o $@ main.o asm_gas.o asm_asasm.o

# It is recommended to use GAS via the gcc frontend by specifying the input
# language 'assembler'.  This has the advantage that you can use the
# preprocessor facilities.
asm_gas.o: asm_gas.s
	$(CC) $(CFLAGS) -x assembler -o $@ -c $<

# AsAsm needs to be invoked directly.
asm_asasm.o: asm_asasm.s
	$(ASASM) -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
