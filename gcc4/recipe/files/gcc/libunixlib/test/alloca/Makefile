CC = arm-unknown-riscos-gcc
CFLAGS = -std=c99 -static
ELF2AIF = elf2aif
RM = rm

TARGETS = dyn_arr,ff8

all: $(TARGETS)

clean:
	$(RM) $(TARGETS)

.PHONY: all clean

dyn_arr,ff8: dyn_arr.c
	$(CC) $(CFLAGS) -o $@ $<
	$(ELF2AIF) $@
