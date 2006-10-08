include ../../Version.mk
include ../../Config.mk
include ../../../../config.mk

OBJDIR = ../../../$(BUILDDIR)/dynamic

CFLAGS += -I.. -I../$(ARCH) -DNO_UNDERSCORE -DVERBOSE_DLINKER -DUSE_CACHE
CFLAGS += -fPIC -D__PIC__ -ffixed-sl -mpic-register=v4

OBJS	= $(OBJDIR)/dlib.o

LIBDL = $(OBJDIR)/libdl.1.9.9.so,e1f

ELF_LDFLAGS=--shared -nostartfiles -nostdlib -fPIC # using GNU ld

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(LIBDL): $(OBJS)
	$(CC) $(OBJS) $(ELF_LDFLAGS) -o $(LIBDL) -Wl,-soname -Wl,libdl.so.$(VMAJOR) -lpic -lUnixLib
#	ln -s system.libdl/1/9/9/so DSOLib:lib.libdl/so
#	ln -s system.libdl/1/9/9/so DSOLib:lib.libdl/so/$(VMAJOR)

obj: $(OBJS)
