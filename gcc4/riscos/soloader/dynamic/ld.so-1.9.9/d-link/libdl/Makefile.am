lddir = $(prefix)/lib

if ARM_EABI
ld_PROGRAMS = libdl.2.0.0.so
else
ld_PROGRAMS = libdl.1.9.9.so
endif
ld_LIBRARIES = libdl.a

libdl_1_9_9_so_SOURCES = dlib.c
libdl_1_9_9_so_CPPFLAGS = -I$(top_srcdir)/d-link -I$(top_srcdir)/d-link/riscos
libdl_1_9_9_so_CFLAGS = -fPIC -O4 -Wall -std=gnu99
libdl_1_9_9_so_LDFLAGS = --shared -nostartfiles -nostdlib -Wl,-soname -Wl,libdl.so.1

libdl_2_0_0_so_SOURCES = dlib.c
libdl_2_0_0_so_CPPFLAGS = -I$(top_srcdir)/d-link -I$(top_srcdir)/d-link/riscos
libdl_2_0_0_so_CFLAGS = -fPIC -O4 -Wall -std=gnu99
libdl_2_0_0_so_LDFLAGS = --shared -nostartfiles -nostdlib -Wl,-soname -Wl,libdl.so.2

libdl_a_SOURCES = static.c
libdl_a_CFLAGS = -O4 -Wall

if ARM_EABI
install-data-hook:
	ln -s -f libdl.2.0.0.so $(prefix)/lib/libdl.so
	ln -s -f libdl.2.0.0.so $(prefix)/lib/libdl.so.2
else
install-data-hook:
        ifeq ($(BUILD_CROSS),1)
	ln -s -f libdl.1.9.9.so $(prefix)/lib/libdl.so
	ln -s -f libdl.1.9.9.so $(prefix)/lib/libdl.so.1
        else
	$(LN) -s -f libdl/1/9/9/so $(prefix)/lib/libdl.so,1c8
	$(LN) -s -f libdl/1/9/9/so $(prefix)/lib/libdl.so.1,1c8
	test -f $(prefix)/lib/libdl.1.9.9.so && mv $(prefix)/lib/libdl.1.9.9.so $(prefix)/lib/libdl.1.9.9.so,e1f
	test -f $(prefix)/lib/libdl.a && rm $(prefix)/lib/libdl.a
        endif
endif
