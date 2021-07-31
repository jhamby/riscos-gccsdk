SUBDIRS = . libdl

if ARM_EABI
LIBRARY_NAME=ld-riscos-eabihf.so
else
LIBRARY_NAME=ld-1.9.9.so
endif

lddir = $(prefix)/lib

AUTOMAKE_OPTIONS = nostdinc

AM_CCASFLAGS = -xassembler-with-cpp -DNO_UNDERSCORE

if !ARM_EABI
OBJCOPY = arm-unknown-riscos-objcopy

# It's necessary to compile boot1.c with -mno-apcs-stack-check. Building it into
# a separate library allows it to have different compile flags from the rest of
# the source. Because stack checking is disabled for this file, GCC will normally
# use sl(r10) as a general register which we don't want, so -ffixed-sl is used to
# stop GCC from corrupting the stack limit register.
noinst_LIBRARIES = libboot1.a

libboot1_a_SOURCES = boot1.c
libboot1_a_CPPFLAGS = -I$(top_srcdir)/d-link -I$(top_srcdir)/d-link/riscos \
		      -DNO_UNDERSCORE -DVERBOSE_DLINKER
libboot1_a_CFLAGS = -fPIC -Wall -O4 -std=gnu99 -mno-apcs-stack-check -ffixed-sl
endif

# We're not using libtool as the dynamic linker is not a typical
# shared library.
ld_PROGRAMS = $(LIBRARY_NAME)

ld_1_9_9_so_SOURCES = \
		hash.c \
		readelflib1.c \
		vsprintf.c \
		../../../../../srcdir/gcc/libunixlib/common/riscosify.c \
		riscos/resolve.s \
		riscos/elfinterp.c \
		riscos/stkovf.s \
		riscos/dl_open.c \
		riscos/dl_stat.c \
		riscos/dl_unwind.c \
		riscos/dl_misc.c \
		riscos/dl_start.s

ld_1_9_9_so_CPPFLAGS = -I$(top_srcdir)/d-link -I$(top_srcdir)/d-link/riscos \
		      -DNO_UNDERSCORE -DVERBOSE_DLINKER -D__TARGET_SOLOADER__
ld_1_9_9_so_CFLAGS = -fPIC -O4 -Wall -std=gnu99
ld_1_9_9_so_LDFLAGS = libboot1.a --shared -Wl,--no-undefined -nostdlib -nostartfiles -Wl,-e -Wl,_dl_start \
			-Wl,-soname -Wl,ld-riscos/so/1

ld_riscos_eabihf_so_SOURCES = \
		boot1.c = \
		hash.c \
		readelflib1.c \
		vsprintf.c \
		../../libunixlib/common/riscosify.c \
		riscos/resolve.s \
		riscos/elfinterp.c \
		riscos/dl_open.c \
		riscos/dl_stat.c \
		riscos/dl_unwind.c \
		riscos/dl_misc.c \
		riscos/dl_start.s

ld_riscos_eabihf_so_CPPFLAGS = -I$(top_srcdir)/d-link -I$(top_srcdir)/d-link/riscos \
		      -DNO_UNDERSCORE -DVERBOSE_DLINKER -D__TARGET_SOLOADER__
ld_riscos_eabihf_so_CFLAGS = -fPIC -O4 -Wall -std=gnu99 -march=armv7-a+fp+neon
ld_riscos_eabihf_so_LDFLAGS = --shared -Wl,--no-undefined -nostdlib -nostartfiles -Wl,-e -Wl,_dl_start \
			-Wl,-soname -Wl,ld-riscos/so/2

if !ARM_EABI
# We have to override the automake target and link manually as it's the only
# way that objcopy can be applied to each object before linking.
ld-1.9.9.so: $(ld_1_9_9_so_OBJECTS) Makefile libboot1.a
	for object in *.o; do \
	  if arm-unknown-riscos-nm $$object | grep -q "__rt_stkovf_split_"; then \
	    echo Processing $$object with $(OBJCOPY); \
	    $(OBJCOPY) --redefine-sym __rt_stkovf_split_small=_dl_stkovf_split_small \
		       --redefine-sym __rt_stkovf_split_big=_dl_stkovf_split_big \
		       $$object $$object; \
	  fi \
	done
	$(CC) -o ld-1.9.9.so $(ld_1_9_9_so_LDFLAGS) $(ld_1_9_9_so_OBJECTS)
endif

if ARM_EABI
install-data-hook:
	ln -s -f ld-riscos-eabihf.so $(prefix)/lib/ld-riscos.so.2
else
install-data-hook:
        ifeq ($(BUILD_CROSS),1)
	ln -s -f ld-1.9.9.so $(prefix)/lib/ld-riscos.so.1
        else
	$(LN) -s -f ld-1/9/9/so $(prefix)/lib/ld-riscos.so.1,1c8
	test -f $(prefix)/lib/ld-1.9.9.so && mv $(prefix)/lib/ld-1.9.9.so $(prefix)/lib/ld-1.9.9.so,e1f
        endif
endif
