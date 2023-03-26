#
# This file is part of Sargasso, http://zamez.org/sargasso
# Licensed under the GNU General Public License,
#                http://www.opensource.org/licenses/gpl-license
#

OBJECTS = feed.o sargasso.o
HEADERS = feed.h

WARNFLAGS = -W -Wall -Wundef -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations -Wredundant-decls \
	-Wnested-externs -Winline -Wno-unused-parameter -Wuninitialized

CC = gcc
# Add -DFORTIFY and/or -DDEBUG for testing
CFLAGS = -O3 -std=c9x -DANSI_PROTOTYPES -D_BSD_SOURCE -D_POSIX_C_SOURCE -Driscos -DBOOL_DEFINED \
	$(WARNFLAGS) `ro-pkg-config --cflags libxml-2.0` -I..
LDFLAGS = -static -L$(GCCSDK_INSTALL_ENV)/lib -static -lssl -lcrypto -lxml2 -lcurl -lOSLib32 -lrufl -lz -lcares -lpcre -lnghttp2

!RunImage,e1f: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c $(HEADERS)
	@echo "==> $<"
	@$(CC) -o $@ -c $(CFLAGS) $<

clean:
	-rm -f feed.o sargasso.o !RunImage,e1f
