

CC=$(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-gcc
CFLAGS=-O2 -Wall -W -mlibscl -lDesk-scl -L$(GCCSDK_INSTALL_ENV)/lib

.PHONY: Decor
Decor: Decor.c
	$(CC) $< $(CFLAGS) -o $$'!Decor/Decor,ff8'

distclean:
