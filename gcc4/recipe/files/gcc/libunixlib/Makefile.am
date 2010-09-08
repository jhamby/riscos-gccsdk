# Automake style makefile for UnixLib
# Copyright (c) 2005-2010 UnixLib developers
# Written by Nick Burrett <nick@sqrt.co.uk>

# Directory where headers will be installed.
tooldir = $(exec_prefix)/$(target_alias)

# Multilib support
MAKEOVERRIDES=

# These flags exclusively apply to a C compiler
WARNINGSC = -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs

WARNINGS = -pedantic -Wall -Wundef -Wpointer-arith \
	-Wcast-align -Wwrite-strings -Winline -Wno-unused \
	-Winline -Wno-unused -W -Wcast-qual -Wshadow $(WARNINGSC)

# The -isystem command tells GCC to prefer the header files in UnixLib,
# rather than trying to compile the library with the system headers.
# Header files that provide additional definitions only required for the
# building of UnixLib should be held in the source directories.
AM_CPPFLAGS = -isystem $(top_srcdir)/include -I $(top_srcdir)/incl-local

# C99 mode is required to build UnixLib because the library now contains
# a lot of C99 features.
# The __GNU_LIBRARY__ definition is for matching the expectation of the couple
# of GNU C Library files we're using.
# The NO_LONG_DOUBLE definition here and the __NO_LONG_DOUBLE_MATH definition
# in features.h results in having the 'long double' math routines aliased as
# the normal 'double' onces.
AM_CFLAGS = -D__GNU_LIBRARY__ -DNO_LONG_DOUBLE -D_GNU_SOURCE=1 -std=c99 $(LIBM_FLAGS)

# Assembler files rely heavily on the C preprocessor to keep structures
# referred to by both languages in sync.
AM_CCASFLAGS = -xassembler-with-cpp -isystem $(top_srcdir)/include -I $(top_srcdir)/incl-local

SUBDIRS = test

includedir = $(prefix)

if BUILDING_SCL
assert_src =
else
assert_src = \
 	assert/assert.c
endif

if BUILDING_SCL
common_src =
else
common_src = \
	common/attrib.c \
	common/cvttime.c \
	common/exist.c \
	common/fdname.c \
	common/funcallerr.c \
	common/objattr.c \
	common/prefix.c \
	common/riscosify.c \
	common/serial.c \
	common/sockvalid.c \
	common/symlinks.c \
	common/unixify.c \
	common/_atomic.s \
	common/_exist.s \
	common/_riscosify.s \
	common/_taskhandle.s \
	common/__stat.c
endif
common_src += \
	common/env.c

if BUILDING_SCL
crypt_src =
else
crypt_src = \
	crypt/crypt-entry.c \
	crypt/crypt.c \
	crypt/crypt_util.c \
	crypt/md5-crypt.c \
	crypt/md5.c
endif

if BUILDING_SCL
ctype_src =
else
ctype_src = \
	ctype/ctypetable.c \
	ctype/isalnum.c \
	ctype/isalpha.c \
	ctype/isascii.c \
	ctype/iscntrl.c \
	ctype/isdigit.c \
	ctype/isgraph.c \
	ctype/islower.c \
	ctype/isprint.c \
	ctype/ispunct.c \
	ctype/isspace.c \
	ctype/isupper.c \
	ctype/isxdigit.c \
	ctype/toascii.c \
	ctype/_tolower.c \
	ctype/tolower.c \
	ctype/_toupper.c \
	ctype/toupper.c
endif

if BUILDING_SCL
debug_src =
else
debug_src = \
	debug/debug.c \
	debug/dvsprintf.c
endif

if BUILDING_SCL
fenv_src =
else
fenv_src = \
	fenv/fclrexcpt.c \
	fenv/fedisblxcpt.c \
	fenv/feenablxcpt.c \
	fenv/fegetenv.c \
	fenv/fegetexcept.c \
	fenv/fegetround.c \
	fenv/feholdexcpt.c \
	fenv/fesetenv.c \
	fenv/fesetround.c \
	fenv/fraiseexcpt.c \
	fenv/fsetexcptflg.c \
	fenv/ftestexcept.c
endif

if BUILDING_SCL
gcccompat_src =
else
gcccompat_src =
endif
gcccompat_src += \
	gcccompat/galloca.c \
	gcccompat/_b_frame.s \
	gcccompat/_b_return.s \
	gcccompat/_eh_support.s \
	gcccompat/_galloca.s

if BUILDING_SCL
gdtoa_src =
else
gdtoa_src = \
	gdtoa/dmisc.c \
	gdtoa/dtoa.c \
	gdtoa/gdtoa.c \
	gdtoa/gethex.c \
	gdtoa/glue.c \
	gdtoa/gmisc.c \
	gdtoa/g_ddfmt.c \
	gdtoa/g_dfmt.c \
	gdtoa/g_ffmt.c \
	gdtoa/g_Qfmt.c \
	gdtoa/g_xfmt.c \
	gdtoa/g_xLfmt.c \
	gdtoa/g__fmt.c \
	gdtoa/hd_init.c \
	gdtoa/hexnan.c \
	gdtoa/misc.c \
	gdtoa/smisc.c \
	gdtoa/strtod.c \
	gdtoa/strtodg.c \
	gdtoa/strtodI.c \
	gdtoa/strtof.c \
	gdtoa/strtoId.c \
	gdtoa/strtoIdd.c \
	gdtoa/strtoIf.c \
	gdtoa/strtoIg.c \
	gdtoa/strtoIQ.c \
	gdtoa/strtoIx.c \
	gdtoa/strtoIxL.c \
	gdtoa/strtopd.c \
	gdtoa/strtopdd.c \
	gdtoa/strtopf.c \
	gdtoa/strtopQ.c \
	gdtoa/strtopx.c \
	gdtoa/strtopxL.c \
	gdtoa/strtord.c \
	gdtoa/strtordd.c \
	gdtoa/strtorf.c \
	gdtoa/strtorQ.c \
	gdtoa/strtorx.c \
	gdtoa/strtorxL.c \
	gdtoa/sum.c \
	gdtoa/ulp.c \
	gdtoa/_hdtoa.c \
	gdtoa/_ldtoa.c
endif

if BUILDING_SCL
gmon_src =
else
gmon_src = \
	gmon/gmon-start.c \
	gmon/gmon.c \
 	gmon/machine-gmon.c \
	gmon/mcount.c \
	gmon/prof-freq.c \
	gmon/profil.c \
	gmon/_profile.s
endif

if BUILDING_SCL
grp_src =
else
grp_src = \
	grp/fgetgrent.c \
	grp/fgetgrentr.c \
	grp/getgrent.c \
	grp/getgrgid.c \
	grp/getgrgid_r.c \
	grp/getgrnam.c \
	grp/getgrnam_r.c \
	grp/getgrouplist.c \
	grp/getgroups.c \
	grp/grpread.c
endif

if BUILDING_SCL
inttypes_src =
else
inttypes_src = \
	inttypes/strtoimax.c \
	inttypes/strtoumax.c
endif

# UnixLib implementation of SharedCLibrary _kernel_*() routines:
if BUILDING_SCL
kernel_src =
else
kernel_src = \
	kernel/kcurskch.s \
	kernel/kfpavailable.s \
	kernel/kgetenv.s \
	kernel/kosargs.s \
	kernel/kosbget.s \
	kernel/kosbput.s \
	kernel/kosbyte.s \
	kernel/koscli.s \
	kernel/koserror.c \
	kernel/kosfile.s \
	kernel/kosfind.s \
	kernel/kosgbpb.s \
	kernel/kosrdch.s \
	kernel/kosword.s \
	kernel/ksetenv.s \
	kernel/kswi.s \
	kernel/kswi_c.s
endif

if BUILDING_SCL
libm_complex_src =
else
libm_complex_src = \
	libm-complex/cabs.c \
	libm-complex/cabsf.c \
	libm-complex/carg.c \
	libm-complex/cargf.c \
	libm-complex/cimag.c \
	libm-complex/cimagf.c \
	libm-complex/conj.c \
	libm-complex/conjf.c \
	libm-complex/creal.c \
	libm-complex/crealf.c
endif

if BUILDING_SCL
libm_dbl64_src =
else
libm_dbl64_src = \
	libm-dbl64/branred.c \
	libm-dbl64/doasin.c \
	libm-dbl64/dosincos.c \
	libm-dbl64/e_acosh.c \
	libm-dbl64/e_asin.c \
	libm-dbl64/e_atan2.c \
	libm-dbl64/e_atanh.c \
	libm-dbl64/e_cosh.c \
	libm-dbl64/e_exp.c \
	libm-dbl64/e_exp2.c \
	libm-dbl64/e_fmod.c \
	libm-dbl64/e_gamma_r.c \
	libm-dbl64/e_hypot.c \
	libm-dbl64/e_j0.c \
	libm-dbl64/e_j1.c \
	libm-dbl64/e_jn.c \
	libm-dbl64/e_lgamma_r.c \
	libm-dbl64/e_log.c \
	libm-dbl64/e_log10.c \
	libm-dbl64/e_log2.c \
	libm-dbl64/e_pow.c \
	libm-dbl64/e_remainder.c \
	libm-dbl64/e_rem_pio2.c \
	libm-dbl64/e_sinh.c \
	libm-dbl64/e_sqrt.c \
	libm-dbl64/halfulp.c \
	libm-dbl64/k_rem_pio2.c \
	libm-dbl64/k_tan.c \
	libm-dbl64/mpa.c \
	libm-dbl64/mpatan.c \
	libm-dbl64/mpatan2.c \
	libm-dbl64/mpexp.c \
	libm-dbl64/mplog.c \
	libm-dbl64/mpsqrt.c \
	libm-dbl64/mptan.c \
	libm-dbl64/sincos32.c \
	libm-dbl64/slowexp.c \
	libm-dbl64/slowpow.c \
	libm-dbl64/s_asinh.c \
	libm-dbl64/s_atan.c \
	libm-dbl64/s_cbrt.c \
	libm-dbl64/s_ceil.c \
	libm-dbl64/s_copysign.c \
	libm-dbl64/s_erf.c \
	libm-dbl64/s_expm1.c \
	libm-dbl64/s_fabs.c \
	libm-dbl64/s_finite.c \
	libm-dbl64/s_floor.c \
	libm-dbl64/s_fpclassify.c \
	libm-dbl64/s_frexp.c \
	libm-dbl64/s_ilogb.c \
	libm-dbl64/s_isinf.c \
	libm-dbl64/s_isnan.c \
	libm-dbl64/s_llrint.c \
	libm-dbl64/s_llround.c \
	libm-dbl64/s_log1p.c \
	libm-dbl64/s_logb.c \
	libm-dbl64/s_lrint.c \
	libm-dbl64/s_lround.c \
	libm-dbl64/s_modf.c \
	libm-dbl64/s_nearbyint.c \
	libm-dbl64/s_remquo.c \
	libm-dbl64/s_rint.c \
	libm-dbl64/s_round.c \
	libm-dbl64/s_scalbln.c \
	libm-dbl64/s_scalbn.c \
	libm-dbl64/s_signbit.c \
	libm-dbl64/s_sin.c \
	libm-dbl64/s_sincos.c \
	libm-dbl64/s_tan.c \
	libm-dbl64/s_tanh.c \
	libm-dbl64/s_trunc.c \
	libm-dbl64/t_exp.c \
	libm-dbl64/w_exp.c
endif

if BUILDING_SCL
libm_flt32_src =
else
libm_flt32_src = \
	libm-flt32/e_acosf.c \
	libm-flt32/e_acoshf.c \
	libm-flt32/e_asinf.c \
	libm-flt32/e_atan2f.c \
	libm-flt32/e_atanhf.c \
	libm-flt32/e_coshf.c \
	libm-flt32/e_exp2f.c \
	libm-flt32/e_expf.c \
	libm-flt32/e_fmodf.c \
	libm-flt32/e_gammaf_r.c \
	libm-flt32/e_hypotf.c \
	libm-flt32/e_j0f.c \
	libm-flt32/e_j1f.c \
	libm-flt32/e_jnf.c \
	libm-flt32/e_lgammaf_r.c \
	libm-flt32/e_log10f.c \
	libm-flt32/e_log2f.c \
	libm-flt32/e_logf.c \
	libm-flt32/e_powf.c \
	libm-flt32/e_remainderf.c \
	libm-flt32/e_rem_pio2f.c \
	libm-flt32/e_sinhf.c \
	libm-flt32/e_sqrtf.c \
	libm-flt32/k_cosf.c \
	libm-flt32/k_rem_pio2f.c \
	libm-flt32/k_sinf.c \
	libm-flt32/k_tanf.c \
	libm-flt32/s_asinhf.c \
	libm-flt32/s_atanf.c \
	libm-flt32/s_cbrtf.c \
	libm-flt32/s_ceilf.c \
	libm-flt32/s_copysignf.c \
	libm-flt32/s_cosf.c \
	libm-flt32/s_erff.c \
	libm-flt32/s_expm1f.c \
	libm-flt32/s_fabsf.c \
	libm-flt32/s_finitef.c \
	libm-flt32/s_floorf.c \
	libm-flt32/s_fpclassifyf.c \
	libm-flt32/s_frexpf.c \
	libm-flt32/s_ilogbf.c \
	libm-flt32/s_isinff.c \
	libm-flt32/s_isnanf.c \
	libm-flt32/s_llrintf.c \
	libm-flt32/s_llroundf.c \
	libm-flt32/s_log1pf.c \
	libm-flt32/s_logbf.c \
	libm-flt32/s_lrintf.c \
	libm-flt32/s_lroundf.c \
	libm-flt32/s_modff.c \
	libm-flt32/s_nearbyintf.c \
	libm-flt32/s_nextafterf.c \
	libm-flt32/s_remquof.c \
	libm-flt32/s_rintf.c \
	libm-flt32/s_roundf.c \
	libm-flt32/s_scalblnf.c \
	libm-flt32/s_scalbnf.c \
	libm-flt32/s_signbitf.c \
	libm-flt32/s_sincosf.c \
	libm-flt32/s_sinf.c \
	libm-flt32/s_tanf.c \
	libm-flt32/s_tanhf.c \
	libm-flt32/s_truncf.c \
	libm-flt32/w_expf.c
endif

if BUILDING_SCL
libm_generic_src =
else
libm_generic_src = \
	libm-generic/e_exp10.c \
	libm-generic/e_exp10f.c \
	libm-generic/e_gammal_r.c \
	libm-generic/e_scalb.c \
	libm-generic/e_scalbf.c \
	libm-generic/s_cacos.c \
	libm-generic/s_cacosf.c \
	libm-generic/s_cacosh.c \
	libm-generic/s_cacoshf.c \
	libm-generic/s_casin.c \
	libm-generic/s_casinf.c \
	libm-generic/s_casinh.c \
	libm-generic/s_casinhf.c \
	libm-generic/s_catan.c \
	libm-generic/s_catanf.c \
	libm-generic/s_catanh.c \
	libm-generic/s_catanhf.c \
	libm-generic/s_ccos.c \
	libm-generic/s_ccosf.c \
	libm-generic/s_ccosh.c \
	libm-generic/s_ccoshf.c \
	libm-generic/s_cexp.c \
	libm-generic/s_cexpf.c \
	libm-generic/s_clog.c \
	libm-generic/s_clog10.c \
	libm-generic/s_clog10f.c \
	libm-generic/s_clogf.c \
	libm-generic/s_cpow.c \
	libm-generic/s_cpowf.c \
	libm-generic/s_cproj.c \
	libm-generic/s_cprojf.c \
	libm-generic/s_csin.c \
	libm-generic/s_csinf.c \
	libm-generic/s_csinh.c \
	libm-generic/s_csinhf.c \
	libm-generic/s_csqrt.c \
	libm-generic/s_csqrtf.c \
	libm-generic/s_ctan.c \
	libm-generic/s_ctanf.c \
	libm-generic/s_ctanh.c \
	libm-generic/s_ctanhf.c \
	libm-generic/s_fdim.c \
	libm-generic/s_fdimf.c \
	libm-generic/s_fma.c \
	libm-generic/s_fmaf.c \
	libm-generic/s_fmax.c \
	libm-generic/s_fmaxf.c \
	libm-generic/s_fmin.c \
	libm-generic/s_fminf.c \
	libm-generic/s_ldexp.c \
	libm-generic/s_ldexpf.c \
	libm-generic/s_nan.c \
	libm-generic/s_nanf.c \
	libm-generic/s_nextafter.c \
	libm-generic/s_nexttowardf.c \
	libm-generic/s_significand.c \
	libm-generic/s_significandf.c \
	libm-generic/w_acos.c \
	libm-generic/w_acosf.c \
	libm-generic/w_acosh.c \
	libm-generic/w_acoshf.c \
	libm-generic/w_asin.c \
	libm-generic/w_asinf.c \
	libm-generic/w_atan2.c \
	libm-generic/w_atan2f.c \
	libm-generic/w_atanh.c \
	libm-generic/w_atanhf.c \
	libm-generic/w_cosh.c \
	libm-generic/w_coshf.c \
	libm-generic/w_drem.c \
	libm-generic/w_dremf.c \
	libm-generic/w_exp10.c \
	libm-generic/w_exp10f.c \
	libm-generic/w_exp2.c \
	libm-generic/w_exp2f.c \
	libm-generic/w_fmod.c \
	libm-generic/w_fmodf.c \
	libm-generic/w_hypot.c \
	libm-generic/w_hypotf.c \
	libm-generic/w_j0.c \
	libm-generic/w_j0f.c \
	libm-generic/w_j1.c \
	libm-generic/w_j1f.c \
	libm-generic/w_jn.c \
	libm-generic/w_jnf.c \
	libm-generic/w_lgamma.c \
	libm-generic/w_lgammaf.c \
	libm-generic/w_lgammaf_r.c \
	libm-generic/w_lgamma_r.c \
	libm-generic/w_log.c \
	libm-generic/w_log10.c \
	libm-generic/w_log10f.c \
	libm-generic/w_log2.c \
	libm-generic/w_log2f.c \
	libm-generic/w_logf.c \
	libm-generic/w_pow.c \
	libm-generic/w_powf.c \
	libm-generic/w_remainder.c \
	libm-generic/w_remainderf.c \
	libm-generic/w_scalb.c \
	libm-generic/w_scalbf.c \
	libm-generic/w_sinh.c \
	libm-generic/w_sinhf.c \
	libm-generic/w_sqrt.c \
	libm-generic/w_sqrtf.c \
	libm-generic/w_tgamma.c \
	libm-generic/w_tgammaf.c
endif

if BUILDING_SCL
libm_support_src =
else
libm_support_src = \
	libm-support/k_standard.c \
	libm-support/s_lib_version.c \
	libm-support/s_matherr.c \
	libm-support/s_signgam.c
endif

if BUILDING_SCL
locale_src =
else
locale_src = \
	locale/localeconv.c \
	locale/nl_langinfo.c \
	locale/setlocale.c \
	locale/strcoll.c \
	locale/strxfrm.c \
	locale/territory.c
endif
locale_src += \
	locale/iconv.c

if BUILDING_SCL
misc_src =
else
misc_src = \
	misc/dirname.c
endif

if BUILDING_SCL
netlib_src = \
	netlib/scl_getservbyname.c \
	netlib/scl_getservbyport.c
else
netlib_src = \
	netlib/ul_getaddrinfo.c \
	netlib/ul_in6_addr.c \
	netlib/ul_inet_lnaof.c \
	netlib/ul_inet_mkad.c \
	netlib/ul_inet_netof.c \
	netlib/ul_inet_netwo.c \
	netlib/ul_inet_ntoa.c \
	netlib/ul_net.c \
	netlib/ul_proto.c \
	netlib/ul_readline.c \
	netlib/ul_serv.c \
	netlib/ul_socketpair.c
endif
netlib_src += \
	netlib/accept.c \
	netlib/bind.c \
	netlib/connect.c \
	netlib/getpeernam.c \
	netlib/getsocknam.c \
	netlib/getsockopt.c \
	netlib/h_errno.c \
	netlib/listen.c \
	netlib/net_error.c \
	netlib/recv.c \
	netlib/recvfrom.c \
	netlib/recvmsg.c \
	netlib/send.c \
	netlib/sendmsg.c \
	netlib/sendto.c \
	netlib/setsockopt.c \
	netlib/shutdown.c \
	netlib/socket.c

# No pthread support for SharedCLibrary:
if BUILDING_SCL
pthread_scr =
else
pthread_src = \
	pthread/atfork.c \
	pthread/attr.c \
	pthread/cancel.c \
	pthread/cleanup.c \
	pthread/cleanupcall.c \
	pthread/cond.c \
	pthread/condattr.c \
	pthread/context.c \
	pthread/create.c \
	pthread/detach.c \
	pthread/equal.c \
	pthread/error.c \
	pthread/exit.c \
	pthread/join.c \
	pthread/key.c \
	pthread/keydest.c \
	pthread/lock.c \
	pthread/mutex.c \
	pthread/mutexattr.c \
	pthread/newnode.c \
	pthread/once.c \
	pthread/pthinit.c \
	pthread/pthsignal.c \
	pthread/rwlock.c \
	pthread/rwlockattr.c \
	pthread/schedparam.c \
	pthread/self.c \
	pthread/sem.c \
	pthread/testcancel.c \
	pthread/_context.s \
	pthread/_exit.s \
	pthread/_ints.s \
	pthread/_yield.s
endif

if BUILDING_SCL
pwd_src =
else
pwd_src = \
	pwd/fgetpwent.c \
	pwd/fgetpwentr.c \
	pwd/getlogin.c \
	pwd/getpw.c \
	pwd/getpwent.c \
	pwd/getpwnam.c \
	pwd/getpwnam_r.c \
	pwd/getpwuid.c \
	pwd/getpwuid_r.c \
	pwd/putpwent.c \
	pwd/pwdread.c
endif

if BUILDING_SCL
regexp_src =
else
regexp_src = \
	regexp/regcomp.c \
	regexp/regerror.c \
	regexp/regexec.c \
	regexp/regfree.c
endif

if BUILDING_SCL
resolv_src = \
	resolv/scl_gethostbyname.c
else
resolv_src = \
	resolv/ul_base64.c \
	resolv/ul_gethnamaddr.c \
	resolv/ul_herror.c \
	resolv/ul_inet_addr.c \
	resolv/ul_inet_neta.c \
	resolv/ul_inet_net_ntop.c \
	resolv/ul_inet_net_pton.c \
	resolv/ul_inet_ntop.c \
	resolv/ul_inet_pton.c \
	resolv/ul_nsap_addr.c \
	resolv/ul_ns_name.c \
	resolv/ul_ns_netint.c \
	resolv/ul_ns_parse.c \
	resolv/ul_ns_print.c \
	resolv/ul_ns_samedomain.c \
	resolv/ul_ns_ttl.c \
	resolv/ul_opensock.c \
	resolv/ul_res_comp.c \
	resolv/ul_res_data.c \
	resolv/ul_res_debug.c \
	resolv/ul_res_hconf.c \
	resolv/ul_res_init.c \
	resolv/ul_res_mkquery.c \
	resolv/ul_res_query.c \
	resolv/ul_res_send.c
endif

if BUILDING_SCL
resource_src =
else
resource_src = \
	resource/getegid.c \
	resource/geteuid.c \
	resource/getgid.c \
	resource/getpgrp.c \
	resource/getpid.c \
	resource/getppid.c \
	resource/getprior.c \
	resource/getrlimit.c \
	resource/getrusage.c \
	resource/getuid.c \
	resource/initialise.c \
	resource/seteuid.c \
	resource/setgid.c \
	resource/setpgid.c \
	resource/setpgrp.c \
	resource/setprior.c \
	resource/setrlimit.c \
	resource/setsid.c \
	resource/setuid.c \
	resource/ulimit.c
endif

if BUILDING_SCL
sched_src =
else
sched_src = \
	sched/syield.c
endif

# SharedCLibrary stub interface:
if BUILDING_SCL
scl_src = \
	scl/chunkid01.s \
	scl/chunkid02.s \
	scl/chunkid04.s \
	scl/chunkid05.s \
	scl/chunkstub.s \
	scl/redirect_setjmp.s \
	scl/scl_acosf.s \
	scl/scl_asinf.s \
	scl/scl_atan2f.s \
	scl/scl_atanf.s \
	scl/scl_ceilf.s \
	scl/scl_fpclassifyf.s \
	scl/scl_fpclassify.s \
	scl/scl_copysignf.s \
	scl/scl_copysign.s \
	scl/scl_cosf.s \
	scl/scl_coshf.s \
	scl/scl_expf.s \
	scl/scl_fabsf.s \
	scl/scl_floorf.s \
	scl/scl_hypotf.s \
	scl/scl_ldexpf.s \
	scl/scl_log10f.s \
	scl/scl_logf.s \
	scl/scl_modff.s \
	scl/scl_powf.s \
	scl/scl_signbit.s \
	scl/scl_sinf.s \
	scl/scl_sinhf.s \
	scl/scl_tanf.s \
	scl/scl_tanhf.s
else
scl_src =
endif

if BUILDING_SCL
search_src =
else
search_src = \
	search/hsearch.c \
	search/hsearch_r.c \
	search/insremque.c \
	search/lsearch.c \
	search/tsearch.c
endif

if BUILDING_SCL
signal_src =
else
signal_src = \
	signal/init.c \
	signal/kill.c \
	signal/pause.c \
	signal/post.c \
	signal/psignal.c \
	signal/raise.c \
	signal/sigaction.c \
	signal/sigaddset.c \
	signal/sigaltstk.c \
	signal/sigblock.c \
	signal/sigdelset.c \
	signal/sigexec.s \
	signal/siginfo.c \
	signal/sigismembe.c \
	signal/signal.c \
	signal/sigpause.c \
	signal/sigpending.c \
	signal/sigprocmsk.c \
	signal/sigsetmask.c \
	signal/sigsets.c \
	signal/sigstack.c \
	signal/sigsuspend.c \
	signal/sigvec.c \
	signal/sleep.c \
	signal/_demangle.s \
	signal/_signal.s
endif
signal_src += \
	signal/signame.c

if BUILDING_SCL
sound_src =
else
sound_src = \
	sound/dsp.c
endif

if BUILDING_SCL
stat_src =
else
stat_src = \
	stat/mkfifo.c \
	stat/mknod.c
endif

if BUILDING_SCL
stdio_src = \
	stdio/scl_fdopen.c \
	stdio/scl_fileno.c
else
stdio_src = \
	stdio/asprintf.c \
	stdio/clearerr.c \
	stdio/fclose.c \
	stdio/ul_fdopen.c \
	stdio/feof.c \
	stdio/ferror.c \
	stdio/fflush.c \
	stdio/fgets.c \
	stdio/filbuf.c \
	stdio/ul_fileno.c \
	stdio/flsbuf.c \
	stdio/ul_fopen.c \
	stdio/fpos.c \
	stdio/fpurge.c \
	stdio/fputs.c \
	stdio/fread.c \
	stdio/freopen.c \
	stdio/fwrite.c \
	stdio/getc.c \
	stdio/gets.c \
	stdio/getw.c \
	stdio/newstream.c \
	stdio/perror.c \
	stdio/popen.c \
	stdio/printf.c \
	stdio/putc.c \
	stdio/puts.c \
	stdio/putw.c \
	stdio/remove.c \
	stdio/rename.c \
	stdio/scanf.c \
	stdio/setbuf.c \
	stdio/stdio.c \
	stdio/tmpfile.c \
	stdio/ungetc.c \
	stdio/vasprintf.c \
	stdio/vfprintf.c
endif
stdio_src += \
	stdio/getdelim.c

if BUILDING_SCL
stdlib_src =
else
stdlib_src = \
	stdlib/abort.c \
	stdlib/abs.c \
	stdlib/alloc.c \
	stdlib/atexit.c \
	stdlib/atof.c \
	stdlib/atoi.c \
	stdlib/atol.c \
	stdlib/atoll.c \
	stdlib/bsearch.c \
	stdlib/div.c \
	stdlib/drand48.c \
	stdlib/erand48.c \
	stdlib/errno.c \
	stdlib/jrand48.c \
	stdlib/labs.c \
	stdlib/lcong48.c \
	stdlib/llabs.c \
	stdlib/ldiv.c \
	stdlib/lldiv.c \
	stdlib/lrand48.c \
	stdlib/mkstemp.c \
	stdlib/mrand48.c \
	stdlib/msort.c \
	stdlib/nrand48.c \
	stdlib/rand.c \
	stdlib/random.c \
	stdlib/realpath.c \
	stdlib/seed48.c \
	stdlib/srand48.c \
	stdlib/strtol.c \
	stdlib/strtold.c \
	stdlib/strtoll.c \
	stdlib/strtoul.c \
	stdlib/strtoull.c \
	stdlib/system.c \
	stdlib/xpg_basename.c \
	stdlib/_rand48.c
endif

if BUILDING_SCL
string_src =
else
string_src = \
	string/basename.c \
	string/memcmp.c \
	string/memrchr.c \
	string/rawmemchr.c \
	string/strcat.c \
	string/strchr.c \
	string/strchrnul.c \
	string/strcmp.c \
	string/strcspn.c \
	string/strncat.c \
	string/strncmp.c \
	string/strncpy.c \
	string/strndup.c \
	string/strnlen.c \
	string/strpbrk.c \
	string/strrchr.c \
	string/strspn.c \
	string/strstr.c \
	string/strtok.c \
	string/swab.c \
	string/_memcpy.s \
	string/_memset.s \
	string/_strcpy.s \
	string/_strlen.s
endif
string_src += \
	string/memccpy.c \
	string/memchr.c \
	string/stpcpy.c \
	string/stpncpy.c \
	string/strdup.c \
	string/strichr.c \
	string/stricmp.c \
	string/strnicmp.c \
	string/strrichr.c \
	string/strsep.c \
	string/strsignal.c \
	string/strtok_r.c \
	string/_ffs.s \
	string/_ffsll.s

if BUILDING_SCL
sys_src =
else
sys_src = \
	sys/alarm.c \
	sys/brk.c \
	sys/exec.c \
	sys/execl.c \
	sys/execle.c \
	sys/execlp.c \
	sys/execv.c \
	sys/execvp.c \
	sys/mman.c \
	sys/mount.c \
	sys/sigsetjmp.c \
	sys/stackalloc.c \
	sys/vfork.c \
	sys/_getcpuarch.s \
	sys/_jmp.s \
	sys/_mman.s \
	sys/_os.s \
	sys/_sigjmp.s \
	sys/_swi.s \
	sys/_syslib.s \
	sys/_vfork.s
endif
sys_src += \
	sys/errlist.c

if BUILDING_SCL
termios_src =
else
termios_src = \
	termios/cfgetispee.c \
	termios/cfgetospee.c \
	termios/cfsetispee.c \
	termios/cfsetospee.c \
	termios/ctermid.c \
	termios/cuserid.c \
	termios/gtty.c \
	termios/stty.c \
	termios/tcdrain.c \
	termios/tcflow.c \
	termios/tcflush.c \
	termios/tcgetattr.c \
	termios/tcgetpgrp.c \
	termios/tcsendbrk.c \
	termios/tcsetattr.c \
	termios/tcsetpgrp.c \
	termios/termcap.c
endif

if BUILDING_SCL
time_src =
else
time_src = \
	time/asctime.c \
	time/difftime.c \
	time/ctime.c \
	time/getitmr.c \
	time/gmtime.c \
	time/localtime.c \
	time/mktime.c \
	time/setitmr.c \
	time/strftime.c \
	time/time.c \
	time/_clock.s
endif
time_src += \
	time/adjtime.c \
	time/asctime_r.c \
	time/broken.c \
	time/calendconv.c \
	time/clk_getres.c \
	time/clk_gettime.c \
	time/clk_settime.c \
	time/ctime_r.c \
	time/dysize.c \
	time/ftime.c \
	time/gettod.c \
	time/gmtime_r.c \
	time/localtime_r.c \
	time/settod.c \
	time/stdtime.c \
	time/timegm.c \
	time/times.c \
	time/tzset.c \
	time/utimes.c

if BUILDING_SCL
unistd_src =
else
unistd_src = \
	unistd/chown.c \
	unistd/chroot.c \
	unistd/fchdir.c \
	unistd/fchown.c \
	unistd/getpagesiz.c \
	unistd/getpass.c \
	unistd/lchown.c \
	unistd/link.c \
	unistd/lockf.c \
	unistd/readlink.c \
	unistd/symlink.c
endif

if BUILDING_SCL
unix_src = \
	unix/scl_close.c \
	unix/scl_ioctl.c \
	unix/scl_lseek.c \
	unix/scl_putenv.c \
	unix/scl_read.c \
	unix/scl_readv.c \
	unix/scl_select.c \
	unix/scl_setenv.c \
	unix/scl_unsetenv.c \
	unix/scl_write.c \
	unix/scl_writev.c
else
unix_src = \
	unix/access.c \
	unix/alphasort.c \
	unix/chdir.c \
	unix/chmod.c \
	unix/ul_clearenv.c \
	unix/ul_close.c \
	unix/creat.c \
	unix/custom.c \
	unix/dev.c \
	unix/dirent.c \
	unix/dirfd.c \
	unix/dup.c \
	unix/dup2.c \
	unix/fchmod.c \
	unix/fcntl.c \
	unix/features.c \
	unix/fstat.c \
	unix/getcwd.c \
	unix/getdtables.c \
	unix/ul_getenv.c \
	unix/getopt.c \
	unix/getopt1.c \
	unix/getsubopt.c \
	unix/glob.c \
	unix/ul_ioctl.c \
	unix/isatty.c \
	unix/ispipe.c \
	unix/ul_lseek.c \
	unix/lstat.c \
	unix/mkdir.c \
	unix/nice.c \
	unix/open.c \
	unix/pathconf.c \
	unix/pipe.c \
	unix/poll.c \
	unix/proc.c \
	unix/pselect.c \
	unix/ul_putenv.c \
	unix/ul_read.c \
	unix/ul_readv.c \
	unix/reopen.c \
	unix/rmdir.c \
	unix/ul_unsetenv.c \
	unix/ul_select.c \
	unix/ul_setenv.c \
	unix/stat.c \
	unix/statfs.c \
	unix/sync.c \
	unix/sysconf.c \
	unix/syslog.c \
	unix/truncate.c \
	unix/tty.c \
	unix/ttyname.c \
	unix/ttyname_r.c \
	unix/umask.c \
	unix/uname.c \
	unix/unix.c \
	unix/unlink.c \
	unix/utime.c \
	unix/wait.c \
	unix/wait3.c \
	unix/wait4.c \
	unix/waitpid.c \
	unix/ul_write.c \
	unix/ul_writev.c
endif
unix_src += \
	unix/fnmatch.c \
	unix/getdomain.c \
	unix/gethostnam.c \
	unix/setdomain.c \
	unix/seterr.c \
	unix/sethostnam.c

if BUILDING_SCL
wchar_src =
else
wchar_src = \
	wchar/btowc.c \
	wchar/mbrlen.c \
	wchar/mbrtowc.c \
	wchar/mbsinit.c \
	wchar/mbsnrtowcs.c \
	wchar/mbsrtowcs.c \
	wchar/wcpcpy.c \
	wchar/wcpncpy.c \
	wchar/wcrtomb.c \
	wchar/wcscat.c \
	wchar/wcschr.c \
	wchar/wcscmp.c \
	wchar/wcscoll.c \
	wchar/wcscpy.c \
	wchar/wcscspn.c \
	wchar/wcsdup.c \
	wchar/wcslen.c \
	wchar/wcsncat.c \
	wchar/wcsncmp.c \
	wchar/wcsncpy.c \
	wchar/wcsnrtombs.c \
	wchar/wcspbrk.c \
	wchar/wcsrchr.c \
	wchar/wcsrtombs.c \
	wchar/wcsspn.c \
	wchar/wcsstr.c \
	wchar/wcstok.c \
	wchar/wcswidth.c \
	wchar/wctob.c \
	wchar/wcwidth.c \
	wchar/wmemchr.c \
	wchar/wmemcmp.c \
	wchar/wmemcpy.c \
	wchar/wmemmove.c \
	wchar/wmemset.c
endif
wchar_src += \
	wchar/mblen.c \
	wchar/mbstowcs.c \
	wchar/mbtowc.c \
	wchar/wcstombs.c \
	wchar/wctomb.c \
	wchar/wctype.c

libc_headers = \
	alloca.h \
	ar.h \
	arpa/ftp.h \
	arpa/inet.h \
	arpa/nameser.h \
	arpa/nameser_compat.h \
	arpa/telnet.h \
	arpa/tftp.h \
	assert.h \
	bits/byteswap.h \
	bits/cmathcalls.h \
	bits/fenv.h \
	bits/huge_val.h \
	bits/huge_valf.h \
	bits/huge_vall.h \
	bits/in.h \
	bits/inf.h \
	bits/libc-lock.h \
	bits/locale.h \
	bits/mathcalls.h \
	bits/mathdef.h \
	bits/mathinline.h \
	bits/nan.h \
	bits/netdb.h \
	bits/posix_opt.h \
	bits/sched.h \
	bits/signum.h \
	bits/sockaddr.h \
	bits/socket.h \
	bits/wchar.h \
	bits/wordsize.h \
	byteswap.h \
	complex.h \
	crypt.h \
	ctype.h \
	dirent.h \
	dlfcn.h \
	endian.h \
	errno.h \
	fcntl.h \
	fcrypt.h \
	features.h \
	fenv.h \
	fnmatch.h \
	fpu_control.h \
	getopt.h \
	glob.h \
	grp.h \
	iconv.h \
	ieee754.h \
	inttypes.h \
	kernel.h \
	langinfo.h \
	libgen.h \
	limits.h \
	locale.h \
	malloc.h \
	math.h \
	memory.h \
	net/if.h \
	netdb.h \
	netinet/in.h \
	netinet/in_systm.h \
	netinet/ip.h \
	netinet/ip_icmp.h \
	netinet/ip_var.h \
	netinet/tcp.h \
	netinet/udp.h \
	nl_types.h \
	poll.h \
	pthread.h \
	pwd.h \
	regex.h \
	regexp.h \
	resolv.h \
	rpc/netdb.h \
	sched.h \
	search.h \
	semaphore.h \
	setjmp.h \
	sgtty.h \
	signal.h \
	stdint.h \
	stdio.h \
	stdio_ext.h \
	stdlib.h \
	string.h \
	string/byteswap.h \
	strings.h \
	swis.h \
	sys/bitypes.h \
	sys/byteorder.h \
	sys/cdefs.h \
	sys/dir.h \
	sys/elf.h \
	sys/errno.h \
	sys/fcntl.h \
	sys/file.h \
	sys/gmon.h \
	sys/gmon_out.h \
	sys/ioctl.h \
	sys/mman.h \
	sys/mount.h \
	sys/param.h \
	sys/poll.h \
	sys/queue.h \
	sys/resource.h \
	sys/select.h \
	sys/signal.h \
	sys/socket.h \
	sys/soundcard.h \
	sys/stat.h \
	sys/statfs.h \
	sys/syslog.h \
	sys/sysmacros.h \
	sys/termios.h \
	sys/time.h \
	sys/timeb.h \
	sys/times.h \
	sys/types.h \
	sys/uio.h \
	sys/un.h \
	sys/utsname.h \
	sys/vfs.h \
	sys/wait.h \
	sysexits.h \
	syslog.h \
	termcap.h \
	termio.h \
	termios.h \
	time.h \
	ulimit.h \
	unistd.h \
	unixlib/buildoptions.h \
	unixlib/local.h \
	unixlib/types.h \
	utime.h \
	wchar.h \
	wctype.h
	
# stubs.h is generated by gen-auto.pl
libc_headers += unixlib/stubs.h

nobase_noinst_HEADERS = \
	incl-local/dirent.h \
	incl-local/errno.h \
	incl-local/features.h \
	incl-local/grp.h \
	incl-local/internal/asm_dec.s \
	incl-local/internal/defines.h \
	incl-local/internal/dev.h \
	incl-local/internal/elf-macros.s \
	incl-local/internal/fd.h \
	incl-local/internal/local.h \
	incl-local/internal/machine-gmon.h \
	incl-local/internal/math.h \
	incl-local/internal/os.h \
	incl-local/internal/scl-macros.s \
	incl-local/internal/sigstate.h \
	incl-local/internal/swiparams.h \
	incl-local/internal/symlinks.h \
	incl-local/internal/tty.h \
	incl-local/internal/unix.h \
	incl-local/locale.h \
	incl-local/math_private.h \
	incl-local/memcopy.h \
	incl-local/pthread.h \
	incl-local/pwd.h \
	incl-local/resolv.h \
	incl-local/setjmp.h \
	incl-local/signal.h \
	incl-local/stdio.h \
	incl-local/stdlib.h \
	incl-local/sys/cdefs.h \
	incl-local/sys/debug.h \
	incl-local/sys/select.h \
	incl-local/sys/socket.h \
	incl-local/sys/stat.h \
	incl-local/time.h \
	incl-local/unistd.h

libunixlib_la_SOURCES = $(assert_src) $(common_src) $(crypt_src) $(ctype_src) \
	$(debug_src) $(fenv_src) $(gcccompat_src) $(gdtoa_src) $(gmon_src) \
	$(grp_src) $(inttypes_src) $(kernel_src) \
	$(libm_complex_src) $(libm_dbl64_src) $(libm_flt32_src) $(libm_generic_src) $(libm_support_src) \
	$(locale_src) $(misc_src) $(netlib_src) $(pthread_src) $(pwd_src) \
	$(regexp_src) $(resolv_src) $(resource_src) $(sched_src) \
	$(scl_src) $(search_src) $(signal_src) $(sound_src) $(stat_src) \
	$(stdio_src) $(stdlib_src) $(string_src) $(sys_src) \
	$(termios_src) $(time_src) $(unistd_src) $(unix_src) $(wchar_src)

libunixlib_la_LDFLAGS = -Wc,-lgcc_s -Wc,-nostdlib -version-info 5:0:0 -Wl,--version-script -Wl,$(top_srcdir)/vscript

libm_la_SOURCES = libm-support/libm.c

libm_la_LDFLAGS = -Wc,-nostdlib -version-info 1:0:0

toolexeclib_LTLIBRARIES = libunixlib.la libm.la

# crt ----------------------

if BUILDING_SCL
toolexeclib_DATA = crt0-scl.o
else
toolexeclib_DATA = crt0.o gcrt0.o
endif

crt0-scl.o: crt/crt0-scl.s
	$(CCASCOMPILE) -o crt0-scl.o -c `test -f 'crt/crt0-scl.s' || echo '$(srcdir)/'`crt/crt0-scl.s
crt0.o: crt/crt0.s
	$(CCASCOMPILE) -o crt0.o -c `test -f 'crt/crt0.s' || echo '$(srcdir)/'`crt/crt0.s
gcrt0.o: crt/gcrt0.s
	$(CCASCOMPILE) -o gcrt0.o -c `test -f 'crt/gcrt0.s' || echo '$(srcdir)/'`crt/gcrt0.s

# SUL module ----------------

# Build the SharedUnixLibrary module
bin_PROGRAMS = sul

sul.o: module/sul.s
	$(CCASCOMPILE) -o sul.o -c `test -f 'module/sul.s' || echo '$(srcdir)/'`module/sul.s

# The ARM linker cannot change output formats during the link
# stage.  We must use strip to convert the ELF binary into
# a raw binary
sul$(EXEEXT): sul.o
	$(STRIP) -O binary -o sul$(EXEEXT) sul.o

# These rules are copied direct from a generated Makefile.
# The only difference is that stack checking is turned off for these targets.
# Automake doesn't seem to provide a per-object file flag setting function.
brk.lo: sys/brk.c
	if $(LTCOMPILE) -Wc,-mno-apcs-stack-check \
	-MT brk.lo -MD -MP -MF "$(DEPDIR)/brk.Tpo" -c -o brk.lo \
	`test -f 'sys/brk.c' || echo '$(srcdir)/'`sys/brk.c; \
	then mv -f "$(DEPDIR)/brk.Tpo" "$(DEPDIR)/brk.Plo"; \
	else rm -f "$(DEPDIR)/brk.Tpo"; exit 1; fi

stackalloc.lo: sys/stackalloc.c
	if $(LTCOMPILE) -Wc,-mno-apcs-stack-check \
	-MT stackalloc.lo -MD -MP -MF "$(DEPDIR)/stackalloc.Tpo" -c -o stackalloc.lo \
	`test -f 'sys/stackalloc.c' || echo '$(srcdir)/'`sys/stackalloc.c; \
	then mv -f "$(DEPDIR)/stackalloc.Tpo" "$(DEPDIR)/stackalloc.Plo"; \
	else rm -f "$(DEPDIR)/stackalloc.Tpo"; exit 1; fi

dvsprintf.lo: debug/dvsprintf.c
	if $(LTCOMPILE) -Wc,-mno-apcs-stack-check \
	-MT dvsprintf.lo -MD -MP -MF "$(DEPDIR)/dvsprintf.Tpo" -c -o dvsprintf.lo \
	`test -f 'debug/dvsprintf.c' || echo '$(srcdir)/'`debug/dvsprintf.c; \
	then mv -f "$(DEPDIR)/dvsprintf.Tpo" "$(DEPDIR)/dvsprintf.Plo"; \
	else rm -f "$(DEPDIR)/dvsprintf.Tpo"; exit 1; fi

install-data-local: install-headers

install-headers:
	$(mkinstalldirs) $(DESTDIR)${tooldir}/include
	for dir in arpa bits net netinet rpc string sys unixlib; do \
	  $(mkinstalldirs) $(DESTDIR)${tooldir}/include/$${dir} ; done
	for file in ${libc_headers}; do \
	  $(INSTALL_DATA) $(srcdir)/include/$${file} $(DESTDIR)${tooldir}/include/$${file} ; done
