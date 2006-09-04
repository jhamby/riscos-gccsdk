# Makefile for the SharedCLibrary stubs
# Copyright (c) 2000-2005 Nick Burrett
# Copyright (c) UnixLib Developers
#
# This file is part of the RISC OS GCCSDK
#

INCLUDES = -isystem .

CROSS_CFLAGS += -DRO_ENV=\"$(ro_env)\"

libsclobj=$(objdir)/libscl/$(TRG)

$(libsclobj)/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

$(libsclobj)/%.o: %.s
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(CFL)

COMPAT = $(libsclobj)/gcc-compat/divdi3.o \
	$(libsclobj)/gcc-compat/moddi3.o \
	$(libsclobj)/gcc-compat/udivdi3.o \
	$(libsclobj)/gcc-compat/umoddi3.o

STDIO = $(libsclobj)/stdio/fdopen.o

STDLIB = $(libsclobj)/stdlib/div.o \
	$(libsclobj)/stdlib/_exit.o \
	$(libsclobj)/stdlib/strtof.o \
	$(libsclobj)/stdlib/strtold.o \
	$(libsclobj)/stdlib/strtoll.o \
	$(libsclobj)/stdlib/strtoull.o

STRING = $(libsclobj)/string/strdup.o

TIME = $(libsclobj)/time/times.o

MATH = 	$(libsclobj)/math/acosf.o \
	$(libsclobj)/math/acosl.o \
	$(libsclobj)/math/asinf.o \
	$(libsclobj)/math/asinl.o \
	$(libsclobj)/math/atan2f.o \
	$(libsclobj)/math/atan2l.o \
	$(libsclobj)/math/atanf.o \
	$(libsclobj)/math/atanl.o \
	$(libsclobj)/math/ceilf.o \
	$(libsclobj)/math/ceill.o \
	$(libsclobj)/math/copysign.o \
	$(libsclobj)/math/copysignf.o \
	$(libsclobj)/math/cosf.o \
	$(libsclobj)/math/coshf.o \
	$(libsclobj)/math/coshl.o \
	$(libsclobj)/math/cosl.o \
	$(libsclobj)/math/expf.o \
	$(libsclobj)/math/expl.o \
	$(libsclobj)/math/fabs.o \
	$(libsclobj)/math/fabsf.o \
	$(libsclobj)/math/fclrexcpt.o \
	$(libsclobj)/math/fegetenv.o \
	$(libsclobj)/math/fegetrnd.o \
	$(libsclobj)/math/feholdexc.o \
	$(libsclobj)/math/fesetenv.o \
	$(libsclobj)/math/fesetroun.o \
	$(libsclobj)/math/finite.o \
	$(libsclobj)/math/floorf.o \
	$(libsclobj)/math/floorl.o \
	$(libsclobj)/math/fraiseexc.o \
	$(libsclobj)/math/fsetexcpt.o \
	$(libsclobj)/math/ftstexcpt.o \
	$(libsclobj)/math/hypot.o \
	$(libsclobj)/math/hypotf.o \
	$(libsclobj)/math/isinf.o \
	$(libsclobj)/math/isnan.o \
	$(libsclobj)/math/ldexpf.o \
	$(libsclobj)/math/ldexpl.o \
	$(libsclobj)/math/log10f.o \
	$(libsclobj)/math/log10l.o \
	$(libsclobj)/math/logf.o \
	$(libsclobj)/math/logl.o \
	$(libsclobj)/math/modf1f.o \
	$(libsclobj)/math/modf1l.o \
	$(libsclobj)/math/powf.o \
	$(libsclobj)/math/powl.o \
	$(libsclobj)/math/signbit.o \
	$(libsclobj)/math/sinf.o \
	$(libsclobj)/math/sinhf.o \
	$(libsclobj)/math/sinhl.o \
	$(libsclobj)/math/sinl.o \
	$(libsclobj)/math/tanf.o \
	$(libsclobj)/math/tanhf.o \
	$(libsclobj)/math/tanhl.o \
	$(libsclobj)/math/tanl.o

LIBC =  $(libsclobj)/unistd.o \
	$(libsclobj)/stubs.o \
	$(libsclobj)/riscosify.o \
	$(libsclobj)/_os.o \
	$(libsclobj)/_riscosify.o 

all:	mkdirs \
	$(ux_gcc_bin_dir)/$(TRG)/libscl.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libscl

.PHONY: mkdirs

mkdirs:
	mkdir -p $(libsclobj)
	mkdir -p $(libsclobj)/gcc-compat
	mkdir -p $(libsclobj)/math
	mkdir -p $(libsclobj)/stdio
	mkdir -p $(libsclobj)/stdlib
	mkdir -p $(libsclobj)/string
	mkdir -p $(libsclobj)/time

install_headers:
	$(insthdr) . "*.h" libscl $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) sys "*.h" libscl/sys $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)

$(ux_gcc_bin_dir)/$(TRG)/libscl.o $(ro_gcc_bin_dir)/$(TRG)/o/libscl: $(LIBC) $(STDLIB) $(STRING) $(MATH) $(COMPAT) $(STDIO) $(TIME)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(LIBC) $(STDLIB) $(STRING) $(MATH) $(COMPAT) $(STDIO) $(TIME)

$(libsclobj)/riscosify.o: ../unixlib/source/common/riscosify.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

$(libsclobj)/unistd.o: unistd.c unistd.h
$(libsclobj)/fcntl.o: fcntl.c fcntl.h
$(libsclobj)/stubs.o: stubs.s
$(libsclobj)/_os.o: _os.s

$(libsclobj)/gcc-compat/divdi3.o: gcc-compat/divdi3.s
$(libsclobj)/gcc-compat/moddi3.o: gcc-compat/moddi3.s
$(libsclobj)/gcc-compat/udivdi3.o: gcc-compat/udivdi3.s
$(libsclobj)/gcc-compat/umoddi3.o: gcc-compat/umoddi3.s

$(libsclobj)/stdio/fdopen.o: stdio/fdopen.c

$(libsclobj)/stdlib/div.o: stdlib/div.c
$(libsclobj)/stdlib/_exit.o: stdlib/_exit.c
$(libsclobj)/stdlib/strtof.o: stdlib/strtof.c
$(libsclobj)/stdlib/strtold.o: stdlib/strtold.c
$(libsclobj)/stdlib/strtoll.o: stdlib/strtoll.c
$(libsclobj)/stdlib/strtoull.o: stdlib/strtoull.c

$(libsclobj)/string/strdup.o: string/strdup.c

$(libsclobj)/time/times.o: time/times.c

$(libsclobj)/math/acosf.o: math/acosf.s
$(libsclobj)/math/acosl.o: math/acosl.s
$(libsclobj)/math/asinf.o: math/asinf.s
$(libsclobj)/math/asinl.o: math/asinl.s
$(libsclobj)/math/atan2f.o: math/atan2f.s
$(libsclobj)/math/atan2l.o: math/atan2l.s
$(libsclobj)/math/atanf.o: math/atanf.s
$(libsclobj)/math/atanl.o: math/atanl.s
$(libsclobj)/math/ceilf.o: math/ceilf.s
$(libsclobj)/math/ceill.o: math/ceill.s
$(libsclobj)/math/copysign.o: math/copysign.s
$(libsclobj)/math/copysignf.o: math/copysignf.s
$(libsclobj)/math/cosf.o: math/cosf.s
$(libsclobj)/math/coshf.o: math/coshf.s
$(libsclobj)/math/coshl.o: math/coshl.s
$(libsclobj)/math/cosl.o: math/cosl.s
$(libsclobj)/math/expf.o: math/expf.s
$(libsclobj)/math/expl.o: math/expl.s
$(libsclobj)/math/fabs.o: math/fabs.s
$(libsclobj)/math/fabsf.o: math/fabsf.s
$(libsclobj)/math/fclrexcpt.o: math/fclrexcpt.s
$(libsclobj)/math/fegetenv.o: math/fegetenv.s
$(libsclobj)/math/fegetrnd.o: math/fegetrnd.s
$(libsclobj)/math/feholdexc.o: math/feholdexc.s
$(libsclobj)/math/fesetenv.o: math/fesetenv.s
$(libsclobj)/math/fesetroun.o: math/fesetroun.s
$(libsclobj)/math/finite.o: math/finite.s
$(libsclobj)/math/floorf.o: math/floorf.s
$(libsclobj)/math/floorl.o: math/floorl.s
$(libsclobj)/math/fraiseexc.o: math/fraiseexc.s
$(libsclobj)/math/fsetexcpt.o: math/fsetexcpt.s
$(libsclobj)/math/ftstexcpt.o: math/ftstexcpt.s
$(libsclobj)/math/hypot.o: math/hypot.s
$(libsclobj)/math/hypotf.o: math/hypotf.s
$(libsclobj)/math/isinf.o: math/isinf.s
$(libsclobj)/math/isnan.o: math/isnan.s
$(libsclobj)/math/ldexpf.o: math/ldexpf.s
$(libsclobj)/math/ldexpl.o: math/ldexpl.s
$(libsclobj)/math/log10f.o: math/log10f.s
$(libsclobj)/math/log10l.o: math/log10l.s
$(libsclobj)/math/logf.o: math/logf.s
$(libsclobj)/math/logl.o: math/logl.s
$(libsclobj)/math/modf1f.o: math/modf1f.s
$(libsclobj)/math/modf1l.o: math/modf1l.s
$(libsclobj)/math/powf.o: math/powf.s
$(libsclobj)/math/powl.o: math/powl.s
$(libsclobj)/math/signbit.o: math/signbit.s
$(libsclobj)/math/sinf.o: math/sinf.s
$(libsclobj)/math/sinhf.o: math/sinhf.s
$(libsclobj)/math/sinhl.o: math/sinhl.s
$(libsclobj)/math/sinl.o: math/sinl.s
$(libsclobj)/math/tanf.o: math/tanf.s
$(libsclobj)/math/tanhf.o: math/tanhf.s
$(libsclobj)/math/tanhl.o: math/tanhl.s
$(libsclobj)/math/tanl.o: math/tanl.s
