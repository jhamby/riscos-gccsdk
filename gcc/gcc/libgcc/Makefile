# Makefile for the GNU Compiler Collection for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000, 2003 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

CC_S = $(CROSS_CC) -xassembler-with-cpp
INCLUDES = -I. -I../../include -I../gcc -I../gcc/config
CROSS_CFLAGS += -D$(targetsystem)
CROSS_CFLAGS += -DIN_GCC -DHAVE_CONFIG_H -DIN_LIBGCC2 -D__GCC_FLOAT_NOT_NEEDED -DDONT_USE_BUILTIN_SETJMP

FPBIT_FLAGS=-DFINE_GRAINED_LIBRARIES -I. -I../../include -I../gcc -I../gcc/config

libobjdir=$(objdir)/gcc-$(VERSION)/libgcc/$(STAGE)/$(TRG)

COMMON = $(libobjdir)/divsi3.o \
        $(libobjdir)/modsi3.o \
        $(libobjdir)/umodsi3.o \
        $(libobjdir)/udivsi3.o \
	$(libobjdir)/div0.o

LIB2ADDEH = $(libobjdir)/unwind-dw2.o \
	$(libobjdir)/unwind-dw2-fde.o \
	$(libobjdir)/unwind-sjlj.o \
	$(libobjdir)/unwind-c.o

LIB2ADDEHDEP = ../gcc/unwind.inc ../gcc/unwind-dw2-fde.h


# All the file that go into the gcc library
LIB2FUNCS_1 = $(libobjdir)/muldi3.o \
	$(libobjdir)/negdi2.o \
	$(libobjdir)/lshrdi3.o \
	$(libobjdir)/ashldi3.o \
	$(libobjdir)/ashrdi3.o \
	$(libobjdir)/ffsdi2.o \
	$(libobjdir)/cmpdi2.o \
	$(libobjdir)/ucmpdi2.o \
	$(libobjdir)/floatdidf.o \
	$(libobjdir)/floatdisf.o \
	$(libobjdir)/fixunssfsi.o \
	$(libobjdir)/fixunsdfdi.o \
	$(libobjdir)/fixdfdi.o \
	$(libobjdir)/fixunssfdi.o \
	$(libobjdir)/fixsfdi.o \
	$(libobjdir)/fixxfdi.o \
	$(libobjdir)/fixunsxfdi.o \
	$(libobjdir)/clz.o \
	$(libobjdir)/clzdi2.o \
	$(libobjdir)/clzsi2.o \
	$(libobjdir)/ctzdi2.o \
	$(libobjdir)/ctzsi2.o \
	$(libobjdir)/popcountsi2.o \
	$(libobjdir)/popcountdi2.o \
	$(libobjdir)/popcount_tab.o \
	$(libobjdir)/paritysi2.o \
	$(libobjdir)/paritydi2.o

LIB2FUNCS_2 = $(libobjdir)/floatdixf.o \
	$(libobjdir)/fixunsxfsi.o \
	$(libobjdir)/fixtfdi.o \
	$(libobjdir)/fixunstfdi.o \
	$(libobjdir)/floatditf.o \
	$(libobjdir)/main.o \
	$(libobjdir)/absvsi2.o \
	$(libobjdir)/absvdi2.o \
	$(libobjdir)/addvsi3.o \
	$(libobjdir)/addvdi3.o \
	$(libobjdir)/subvsi3.o \
	$(libobjdir)/subvdi3.o \
	$(libobjdir)/mulvsi3.o \
	$(libobjdir)/mulvdi3.o \
	$(libobjdir)/negvsi2.o \
	$(libobjdir)/negvdi2.o \
	$(libobjdir)/ctors.o

IEEE754 = $(libobjdir)/fixunsdfsi.o \
	$(libobjdir)/negdf2.o \
	$(libobjdir)/addsubdf3.o \
	$(libobjdir)/muldivdf3.o \
	$(libobjdir)/cmpdf2.o \
	$(libobjdir)/unorddf2.o \
	$(libobjdir)/fixdfsi.o \
	$(libobjdir)/fixunsdfsi.o \
	$(libobjdir)/truncdfsf2.o \
	$(libobjdir)/negsf2.o \
	$(libobjdir)/addsubsf3.o \
	$(libobjdir)/muldivsf3.o \
	$(libobjdir)/cmpsf2.o \
	$(libobjdir)/unordsf2.o \
	$(libobjdir)/fixsfsi.o \
	$(libobjdir)/fixunssfsi.o


LIB2FUNCS_ST = $(libobjdir)/eprintf.o \
	$(libobjdir)/bb.o \
	$(libobjdir)/gcc_bcmp.o

LIB2FUNCS_EXTRA = $(libobjdir)/ffs.o \
	$(libobjdir)/clricache.o \
	$(libobjdir)/libgcc_fix.o \
	$(libobjdir)/dynamic.o \
	$(libobjdir)/nonlocal.o


FP_BIT_DOUBLE = $(libobjdir)/pack_df.o \
	$(libobjdir)/unpack_df.o \
	$(libobjdir)/addsub_df.o \
	$(libobjdir)/mul_df.o \
	$(libobjdir)/div_df.o \
	$(libobjdir)/fpcmp_p_df.o \
	$(libobjdir)/compare_df.o \
	$(libobjdir)/eq_df.o \
	$(libobjdir)/ne_df.o \
	$(libobjdir)/gt_df.o \
	$(libobjdir)/ge_df.o \
	$(libobjdir)/lt_df.o \
	$(libobjdir)/le_df.o \
	$(libobjdir)/unord_df.o \
	$(libobjdir)/si_to_df.o \
	$(libobjdir)/df_to_si.o \
	$(libobjdir)/negate_df.o \
	$(libobjdir)/make_df.o \
	$(libobjdir)/df_to_sf.o \
	$(libobjdir)/thenan_df.o \
	$(libobjdir)/df_to_usi.o \
	$(libobjdir)/usi_to_df.o

FP_BIT_SINGLE = $(libobjdir)/pack_sf.o \
	$(libobjdir)/unpack_sf.o \
	$(libobjdir)/addsub_sf.o \
	$(libobjdir)/mul_sf.o \
	$(libobjdir)/div_sf.o \
	$(libobjdir)/fpcmp_p_sf.o \
	$(libobjdir)/compare_sf.o \
	$(libobjdir)/eq_sf.o \
	$(libobjdir)/ne_sf.o \
	$(libobjdir)/gt_sf.o \
	$(libobjdir)/ge_sf.o \
	$(libobjdir)/lt_sf.o \
	$(libobjdir)/le_sf.o \
	$(libobjdir)/unord_sf.o \
	$(libobjdir)/si_to_sf.o \
	$(libobjdir)/sf_to_si.o \
	$(libobjdir)/negate_sf.o \
	$(libobjdir)/make_sf.o \
	$(libobjdir)/sf_to_df.o \
	$(libobjdir)/thenan.o \
	$(libobjdir)/sf_to_usi.o \
	$(libobjdir)/usi_to_sf.o

all:	$(ux_gcc_bin_dir)/$(TRG)/libgcc.o $(ro_gcc_bin_dir)/$(TRG)/o/libgcc

LIBGCC_OBJS = $(COMMON) $(LIB2FUNCS_1) $(LIB2FUNCS_2) $(IEEE754) \
	$(LIB2FUNCS_EXTRA) $(FP_BIT_DOUBLE) $(FP_BIT_SINGLE) $(LIB2ADDEH)

$(ux_gcc_bin_dir)/$(TRG)/libgcc.o: $(LIBGCC_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(LIBGCC_OBJS)

$(ro_gcc_bin_dir)/$(TRG)/o/libgcc: $(LIBGCC_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(LIBGCC_OBJS)

ieee754-df-aof.S:	ieee754-df.S aof-convert.pl
	./aof-convert.pl $< $@

ieee754-sf-aof.S:	ieee754-sf.S aof-convert.pl
	./aof-convert.pl $< $@

############################################################################

# GCC standard run-time libraries
$(libobjdir)/getopt.o:	../../libiberty/getopt.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c ../../libiberty/getopt.c $(INCLUDES)

$(libobjdir)/getopt1.o:	../../libiberty/getopt1.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c ../../libiberty/getopt1.c $(INCLUDES)

$(libobjdir)/dummy.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL__dummy

$(libobjdir)/gcc_bcmp.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL__gcc_bcmp

$(libobjdir)/eh.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_eh -DDONT_USE_BUILTIN_SETJMP

$(libobjdir)/eprintf.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_eprintf

$(libobjdir)/pure.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_pure

$(libobjdir)/main.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL__main -DCTOR_LISTS_DEFINED_EXTERNALLY

$(libobjdir)/bb.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_bb


$(libobjdir)/negdi2.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_negdi2

$(libobjdir)/ffsdi2.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_ffsdi2


$(libobjdir)/floatdisf.o:	libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_floatdisf

$(libobjdir)/fixdfdi.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_fixdfdi

#$(libobjdir)/fixunssfdi.o: libgcc2.c
#	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_fixunssfdi

#$(libobjdir)/fixsfdi.o: libgcc2.c
#	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_fixsfdi

$(libobjdir)/fixxfdi.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_fixxfdi

$(libobjdir)/fixunsdfdi.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_fixunsdfdi

$(libobjdir)/floatdixf.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_floatdixf

$(libobjdir)/fixunsxfsi.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_fixunsxfsi

$(libobjdir)/fixtfdi.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_fixtfdi

$(libobjdir)/fixunstfdi.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_fixunstfdi

$(libobjdir)/floatditf.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_floatditf


$(libobjdir)/absvsi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_absvsi2
$(libobjdir)/absvdi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_absvdi2
$(libobjdir)/addvsi3.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_addvsi3
$(libobjdir)/addvdi3.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_addvdi3
$(libobjdir)/subvsi3.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_subvsi3
$(libobjdir)/subvdi3.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_subvdi3
$(libobjdir)/mulvsi3.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_mulvsi3
$(libobjdir)/mulvdi3.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_mulvdi3
$(libobjdir)/negvsi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_negvsi2
$(libobjdir)/negvdi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_negvdi2


$(libobjdir)/clz.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_clz
$(libobjdir)/clzdi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_clzdi2
$(libobjdir)/clzsi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_clzsi2
$(libobjdir)/ctzdi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_ctzdi2
$(libobjdir)/ctzsi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_ctzsi2
$(libobjdir)/popcountsi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_popcountsi2
$(libobjdir)/popcountdi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_popcountdi2
$(libobjdir)/popcount_tab.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_popcount_tab
$(libobjdir)/paritysi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_paritysi2
$(libobjdir)/paritydi2.o: libgcc2.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c libgcc2.c $(INCLUDES) -DL_paritydi2

# ieee754 double-precision math functions
$(libobjdir)/negdf2.o: ieee754-df-aof.S
	$(CC_S) -c $< -o $@ -DL_negdf2

$(libobjdir)/addsubdf3.o: ieee754-df-aof.S
	$(CC_S) -c $< -o $@ -DL_addsubdf3

$(libobjdir)/muldivdf3.o: ieee754-df-aof.S
	$(CC_S) -c $< -o $@ -DL_muldivdf3

$(libobjdir)/cmpdf2.o: ieee754-df-aof.S
	$(CC_S) -c $< -o $@ -DL_cmpdf2

$(libobjdir)/unorddf2.o: ieee754-df-aof.S
	$(CC_S) -c $< -o $@ -DL_unorddf2

$(libobjdir)/fixdfsi.o: ieee754-df-aof.S
	$(CC_S) -c $< -o $@ -DL_fixdfsi

$(libobjdir)/fixunsdfsi.o: ieee754-df-aof.S
	$(CC_S) -c $< -o $@ -DL_fixunsdfsi

$(libobjdir)/truncdfsf2.o: ieee754-df-aof.S
	$(CC_S) -c $< -o $@ -DL_truncdfsf2


# ieee754 single-precision math functions
$(libobjdir)/negsf2.o: ieee754-sf-aof.S
	$(CC_S) -c $< -o $@ -DL_negsf2

$(libobjdir)/addsubsf3.o: ieee754-sf-aof.S
	$(CC_S) -c $< -o $@ -DL_addsubsf3

$(libobjdir)/muldivsf3.o: ieee754-sf-aof.S
	$(CC_S) -c $< -o $@ -DL_muldivsf3

$(libobjdir)/cmpsf2.o: ieee754-sf-aof.S
	$(CC_S) -c $< -o $@ -DL_cmpsf2

$(libobjdir)/unordsf2.o: ieee754-sf-aof.S
	$(CC_S) -c $< -o $@ -DL_unordsf2

$(libobjdir)/fixsfsi.o: ieee754-sf-aof.S
	$(CC_S) -c $< -o $@ -DL_fixsfsi

$(libobjdir)/fixunssfsi.o: ieee754-sf-aof.S
	$(CC_S) -c $< -o $@ -DL_fixunssfsi


# 32-bit integer division routines
$(libobjdir)/modsi3.o:	lib1funcs-aof.S
	$(CC_S) -c $< -o $@ -DL_modsi3

$(libobjdir)/umodsi3.o:	lib1funcs-aof.S
	$(CC_S) -c $< -o $@ -DL_umodsi3

$(libobjdir)/divsi3.o:	lib1funcs-aof.S
	$(CC_S) -c $< -o $@ -DL_divsi3

$(libobjdir)/udivsi3.o:	lib1funcs-aof.S
	$(CC_S) -c $< -o $@ -DL_udivsi3


# Exception handling
$(libobjdir)/unwind-dw2.o: ../gcc/unwind-dw2.c $(LIB2ADDEHDEP)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c ../gcc/unwind-dw2.c $(INCLUDES)

$(libobjdir)/unwind-dw2-fde.o: ../gcc/unwind-dw2-fde.c $(LIB2ADDEHDEP)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c ../gcc/unwind-dw2-fde.c $(INCLUDES)

$(libobjdir)/unwind-sjlj.o: ../gcc/unwind-sjlj.c $(LIB2ADDEHDEP)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c ../gcc/unwind-sjlj.c $(INCLUDES) -I../gcc

$(libobjdir)/unwind-c.o: ../gcc/unwind-c.c $(LIB2ADDEHDEP)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c ../gcc/unwind-c.c $(INCLUDES) -I../gcc



# Assembler versions of certain library functions

$(libobjdir)/div0.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_div0
$(libobjdir)/dynamic.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_arm_alloca
$(libobjdir)/clricache.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_clear_icache
$(libobjdir)/ctors.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_ctors
$(libobjdir)/libgcc_fix.o:	libgcc_fix.s
	$(CC_S) -c libgcc_fix.s -o $@
$(libobjdir)/nonlocal.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_nonlocal_goto
$(libobjdir)/cmpdi2.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_cmpdi2
$(libobjdir)/ucmpdi2.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_ucmpdi2
$(libobjdir)/moddi3.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_moddi3
$(libobjdir)/umoddi3.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_umoddi3
$(libobjdir)/divdi3.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_divdi3
$(libobjdir)/udivdi3.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_udivdi3
#$(libobjdir)/floatdisf.o:	lib1aof.s
#	$(CC_S) -c lib1aof.s -o $@ -DL_floatdisf
$(libobjdir)/floatdidf.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_floatdidf
#$(libobjdir)/floatdixf.o:	lib1aof.s
#	$(CC_S) -c lib1aof.s -o $@ -DL_floatdixf
$(libobjdir)/fixunsxfdi.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_fixunsxfdi
#$(libobjdir)/fixunsdfdi.o:	lib1aof.s
#	$(CC_S) -c lib1aof.s -o $@ -DL_fixunsdfdi
$(libobjdir)/fixunssfdi.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_fixunssfdi
$(libobjdir)/fixsfdi.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_fixsfdi
$(libobjdir)/ffs.o:		lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_ffs
$(libobjdir)/ashldi3.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_ashldi3
$(libobjdir)/ashrdi3.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_ashrdi3
$(libobjdir)/lshrdi3.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_lshrdi3
$(libobjdir)/muldi3.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_muldi3
$(libobjdir)/nextarg.o:	lib1aof.s
	$(CC_S) -c lib1aof.s -o $@ -DL_builtin_next_arg

# fp-bit library (double precision)
$(libobjdir)/pack_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_pack_df
$(libobjdir)/unpack_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_unpack_df
$(libobjdir)/addsub_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_addsub_df
$(libobjdir)/mul_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_mul_df
$(libobjdir)/div_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_div_df
$(libobjdir)/fpcmp_p_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_fpcmp_parts_df
$(libobjdir)/compare_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_compare_df
$(libobjdir)/eq_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_eq_df
$(libobjdir)/ne_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_ne_df
$(libobjdir)/gt_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_gt_df
$(libobjdir)/ge_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_ge_df
$(libobjdir)/lt_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_lt_df
$(libobjdir)/le_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_le_df
$(libobjdir)/si_to_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_si_to_df
$(libobjdir)/df_to_si.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_df_to_si
$(libobjdir)/df_to_usi.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_df_to_usi
$(libobjdir)/negate_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_negate_df
$(libobjdir)/unord_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_unord_df
$(libobjdir)/thenan_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_thenan_df
$(libobjdir)/make_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_make_df
$(libobjdir)/usi_to_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_usi_to_df


# fplib (single precision)
$(libobjdir)/pack_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_pack_sf
$(libobjdir)/unpack_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_unpack_sf
$(libobjdir)/addsub_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_addsub_sf
$(libobjdir)/mul_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_mul_sf
$(libobjdir)/div_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_div_sf
$(libobjdir)/fpcmp_p_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_fpcmp_parts_sf
$(libobjdir)/compare_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_compare_sf
$(libobjdir)/eq_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_eq_sf
$(libobjdir)/ne_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_ne_sf
$(libobjdir)/gt_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_gt_sf
$(libobjdir)/ge_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_ge_sf
$(libobjdir)/lt_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_lt_sf
$(libobjdir)/le_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_le_sf
$(libobjdir)/si_to_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_si_to_sf
$(libobjdir)/sf_to_si.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_sf_to_si
$(libobjdir)/sf_to_usi.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_sf_to_usi
$(libobjdir)/negate_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_negate_sf
$(libobjdir)/unord_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_unord_sf
$(libobjdir)/thenan.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_thenan
$(libobjdir)/make_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_make_sf

$(libobjdir)/df_to_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_df_to_sf
$(libobjdir)/sf_to_df.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c -DFLOAT $(FPBIT_FLAGS) -DL_sf_to_df
$(libobjdir)/usi_to_sf.o:	fp-bit.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c fp-bit.c $(FPBIT_FLAGS) -DL_usi_to_sf
