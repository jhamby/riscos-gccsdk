# Makefile for Boehm Garbage Collector for the ARM/RISC OS GCC port
# Copyright (c) 2004 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the GNU Compiler Collection
#

INCLUDES = -I include
gcobjdir=$(objdir)/gcc-$(VERSION)/boehm-gc/$(STAGE)/$(TRG)
CROSS_CFLAGS += -DATOMIC_UNCOLLECTABLE -DNO_SIGNALS -DNO_EXECUTE_PERMISSION -DSILENT -DALL_INTERIOR_POINTERS -DGC_GCJ_SUPPORT

$(gcobjdir)/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

GC_OBJS = $(gcobjdir)/allchblk.o \
	$(gcobjdir)/alloc.o \
	$(gcobjdir)/blacklst.o \
	$(gcobjdir)/checksums.o \
	$(gcobjdir)/dbg_mlc.o \
	$(gcobjdir)/dyn_load.o \
	$(gcobjdir)/finalize.o \
	$(gcobjdir)/gc_dlopen.o \
	$(gcobjdir)/gcj_mlc.o \
	$(gcobjdir)/headers.o \
	$(gcobjdir)/irix_threads.o \
	$(gcobjdir)/linux_threads.o \
	$(gcobjdir)/malloc.o \
	$(gcobjdir)/mallocx.o \
	$(gcobjdir)/mark.o \
	$(gcobjdir)/mark_rts.o \
	$(gcobjdir)/misc.o \
	$(gcobjdir)/new_hblk.o \
	$(gcobjdir)/obj_map.o \
	$(gcobjdir)/os_dep.o \
	$(gcobjdir)/pcr_interface.o \
	$(gcobjdir)/ptr_chck.o \
	$(gcobjdir)/real_malloc.o \
	$(gcobjdir)/reclaim.o \
	$(gcobjdir)/specific.o \
	$(gcobjdir)/stubborn.o \
	$(gcobjdir)/typd_mlc.c \
	$(gcobjdir)/backgraph.o

all:	$(ux_gcc_bin_dir)/$(TRG)/libgc.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libgc

$(ux_gcc_bin_dir)/$(TRG)/libgc.o: $(GC_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(GC_OBJS)

$(ro_gcc_bin_dir)/$(TRG)/o/libgc: $(GC_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(GC_OBJS)

$(gcobjdir)/allchblk.o: allchblk.c
$(gcobjdir)/alloc.o: alloc.c
$(gcobjdir)/blacklst.o: blacklst.c
$(gcobjdir)/checksums.o: checksums.c
$(gcobjdir)/dbg_mlc.o: dbg_mlc.c
$(gcobjdir)/dyn_load.o: dyn_load.c
$(gcobjdir)/finalize.o: finalize.c
$(gcobjdir)/gc_dlopen.o: gc_dlopen.c
$(gcobjdir)/gcj_mlc.o: gcj_mlc.c
$(gcobjdir)/headers.o: headers.c
$(gcobjdir)/irix_threads.o: irix_threads.c
$(gcobjdir)/linux_threads.o: linux_threads.c
$(gcobjdir)/malloc.o: malloc.c
$(gcobjdir)/mallocx.o: mallocx.c
$(gcobjdir)/mark.o: mark.c
$(gcobjdir)/mark_rts.o: mark_rts.c
$(gcobjdir)/misc.o: misc.c
$(gcobjdir)/new_hblk.o: new_hblk.c
$(gcobjdir)/obj_map.o: obj_map.c
$(gcobjdir)/os_dep.o: os_dep.c
$(gcobjdir)/pcr_interface.o: pcr_interface.c
$(gcobjdir)/ptr_chck.o: ptr_chck.c
$(gcobjdir)/real_malloc.o: real_malloc.c
$(gcobjdir)/reclaim.o: reclaim.c
$(gcobjdir)/specific.o: specific.c
$(gcobjdir)/stubborn.o: stubborn.c
$(gcobjdir)/typd_mlc.c: typd_mlc.c
$(gcobjdir)/backgraph.o: backgraph.c
