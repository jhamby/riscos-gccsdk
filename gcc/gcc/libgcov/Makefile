# Makefile for GNU libgcov for the ARM/RISC OS GCC port
#

INCLUDES = -I ../gcc -I ../../include -D__RISCOSIFY -DBITS_PER_UNIT=8

gcovdir=$(objdir)/gcc-$(VERSION)/libgcov/$(STAGE)/$(TRG)


# Modules that comprise the library.
OBJS = $(gcovdir)/gcov.o \
       $(gcovdir)/gcov_merge_add.o \
       $(gcovdir)/gcov_merge_single.o \
       $(gcovdir)/gcov_merge_delta.o

all:	$(ux_gcc_bin_dir)/$(TRG)/libgcov.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libgcov

$(ux_gcc_bin_dir)/$(TRG)/libgcov.o: $(OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS)

$(ro_gcc_bin_dir)/$(TRG)/o/libgcov: $(OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS)

$(gcovdir)/gcov.o: ../gcc/libgcov.c
	$(CROSS_CC) -o $@ -c $< $(CROSS_CFLAGS) $(INCLUDES) $(CFL) \
	-DL_gcov

$(gcovdir)/gcov_merge_add.o: ../gcc/libgcov.c
	$(CROSS_CC) -o $@ -c $< $(CROSS_CFLAGS) $(INCLUDES) $(CFL) \
 	-DL_gcov_merge_add

$(gcovdir)/gcov_merge_single.o: ../gcc/libgcov.c
	$(CROSS_CC) -o $@ -c $< $(CROSS_CFLAGS) $(INCLUDES) $(CFL) \
 	-DL_gcov_merge_single

$(gcovdir)/gcov_merge_delta.o: ../gcc/libgcov.c
	$(CROSS_CC) -o $@ -c $< $(CROSS_CFLAGS) $(INCLUDES) $(CFL) \
 	-DL_gcov_merge_delta

