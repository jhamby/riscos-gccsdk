include ../Version.mk
include ../Config.mk
include ../../../config.mk

OBJDIR = ../../$(BUILDDIR)/dynamic

DIRS = $(ARCH)

CFLAGS += -I. -I./$(ARCH) -DNO_UNDERSCORE -DVERBOSE_DLINKER -DUSE_CACHE \
	-fPIC -D__PIC__ -mpic-register=v4 -ffixed-sl
CFLAGS_EXTRA = -mno-apcs-stack-check
# -g

DLINK_OBJS = $(OBJDIR)/boot1.o $(OBJDIR)/hash.o $(OBJDIR)/readelflib1.o $(OBJDIR)/vsprintf.o

OBJS	= $(DLINK_OBJS)

OBJS += $(OBJDIR)/resolve.o $(OBJDIR)/elfinterp.o $(OBJDIR)/stkovf.o $(OBJDIR)/dl_open.o $(OBJDIR)/dl_stat.o

DLINKER = $(OBJDIR)/ld-1.9.9.so,e1f

ELF_LDFLAGS=--shared -fPIC -Wl,--no-undefined -nostdlib # using GNU ld

.PHONY: $(DIRS)
.PHONY: libdl

all: $(DIRS) $(DLINK_OBJS) $(DLINKER) libdl

$(OBJDIR)/%.o: %.s
	$(CC) $(CFLAGS) -o $@ -c $<
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	$(OBJCOPY) --redefine-sym __rt_stkovf_split_small=_dl_stkovf_split_small --redefine-sym __rt_stkovf_split_big=_dl_stkovf_split_big $@

$(OBJDIR)/boot1.o: boot1.c
	$(CC) $(CFLAGS) $(CFLAGS_EXTRA) -o $@ -c $<

$(DIRS) libdl:
	$(MAKE) -C $@

$(DLINKER): $(OBJS)
	$(CC) $(OBJS) -Wl,-e -Wl,_dl_boot $(ELF_LDFLAGS) -o $(DLINKER) -Wl,-soname -Wl,ld-riscos/so/$(VMAJOR)



$(OBJDIR)/boot1.o: boot1.c link.h riscos/sysdep.h hash.h linuxelf.h riscos/syscall.h riscos/string.h
$(OBJDIR)/hash.o: hash.c libdl/dlfcn.h hash.h linuxelf.h riscos/string.h riscos/sysdep.h
$(OBJDIR)/readelflib1.o: readelflib1.c riscos/sysdep.h riscos/syscall.h riscos/string.h
$(OBJDIR)/elfinterp.o: riscos/elfinterp.c riscos/syscall.h
$(OBJDIR)/resolve.o: riscos/resolve.s
$(OBJDIR)/vsprintf.o: vsprintf.c riscos/syscall.h
