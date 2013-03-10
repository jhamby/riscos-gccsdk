# Makefile to build RISC OS unittests.  Those unittests can also be built for
# other platforms just for reference purposes.
# Copyright (c) 2013 UnixLib Developers.

OUTPUT_RO ?= /tmp/riscos
OUTPUT_NA ?= /tmp/native

all: riscos

TESTS := $(wildcard test-*.c)
EXT_RO := ,ff8
EXT_NA :=
TESTS_RO := $(addprefix $(OUTPUT_RO)/,$(patsubst test-%.c,%$(EXT_RO),$(TESTS)))
TESTS_NA := $(addprefix $(OUTPUT_NA)/,$(patsubst test-%.c,%,$(TESTS)))

riscos: $(TESTS_RO) $(OUTPUT_RO)/RunMe,feb

native: $(TESTS_NA)

clean: riscos-clean native-clean

riscos-clean:
	-rm $(TESTS_RO)

native-clean:
	-rm $(TESTS_NA)

.PHONY: all riscos native clean riscos-clean native-clean

CC_RO := arm-unknown-riscos-gcc
CC_NA := gcc
CFLAGS := -std=c99 -Wall -D_POSIX_C_SOURCE=200809L -D_BSD_SOURCE
CFLAGS_RO := $(CFLAGS) -static
CFLAGS_NA := $(CFLAGS)
LDFLAGS_RO :=
LDFLAGS_NA := -lrt

$(OUTPUT_RO)/%$(EXT_RO) : test-%.c
	-mkdir -p $(OUTPUT_RO)
	$(CC_RO) $(CFLAGS_RO) -o $@ $< $(LDFLAGS_RO) && elf2aif $@

$(OUTPUT_NA)/% : test-%.c
	-mkdir -p $(OUTPUT_NA)
	$(CC_NA) $(CFLAGS_NA) -o $@ $< $(LDFLAGS_NA)

$(OUTPUT_RO)/RunMe,feb: $(TESTS)
	@echo "| Obey file to run the UnixLib unittests in one go" > $@
	@echo $(foreach test,$(patsubst test-%.c,%,$(TESTS)),"Run <Obey\$$Dir>.$(test)\n") >> $@

