ARCH = riscos
#ARCH = i386
#ARCH = m68k
#ARCH = sparc
#DEBUG = true

.SUFFIXES:
.SUFFIXES: .S .c .o .s

ifeq ($(ARCH),sparc)
AOUT_SUPPORT = false
else
ifeq ($(ARCH),riscos)
AOUT_SUPPORT = false
else
AOUT_SUPPORT = true
endif
endif

LDSO_ADDR = 62f00020
LDSO_ENTRY = "0x$(LDSO_ADDR)"

# Do NOT use -fomit-frame-pointer -- It won't work!
CFLAGS	= -Wall -O4 -DVERSION=\"$(VERSION)\" -D_GNU_SOURCE -mpoke-function-name -fcall-used-v5
ifeq ($(DEBUG),true)
CFLAGS  += -DDEBUG
endif

ifeq ($(ARCH),i386)
LIBC5_CC = /usr/i486-linuxlibc1/bin/gcc
else
LIBC5_CC = /usr/$(ARCH)-linuxlibc1/bin/gcc
endif
