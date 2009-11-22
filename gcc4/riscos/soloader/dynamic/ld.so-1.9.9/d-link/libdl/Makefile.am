lddir = $(prefix)/lib

ld_PROGRAMS = libdl.1.9.9.so
ld_LIBRARIES = libdl.a

libdl_1_9_9_so_SOURCES = dlib.c
libdl_1_9_9_so_CPPFLAGS = -I$(top_srcdir)/d-link -I$(top_srcdir)/d-link/riscos -DUSE_CACHE
libdl_1_9_9_so_CFLAGS = -fPIC -O4 -Wall -std=gnu99
libdl_1_9_9_so_LDFLAGS = --shared -nostartfiles -nostdlib -Wl,-soname -Wl,libdl.so.1

libdl_a_SOURCES = static.c
libdl_a_CFLAGS = -O4 -Wall

install-data-hook:
        ifeq ($(BUILD_CROSS),1)
	ln -s -f libdl.1.9.9.so $(prefix)/lib/libdl.so
	ln -s -f libdl.1.9.9.so $(prefix)/lib/libdl.so.1
        else
	$(LN) -s -f libdl/1/9/9/so $(prefix)/lib/libdl.so,1c8
	$(LN) -s -f libdl/1/9/9/so $(prefix)/lib/libdl.so.1,1c8
        endif
