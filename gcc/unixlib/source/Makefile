# Makefile for UnixLib for the ARM/RISC OS GCC port
# Copyright (c) 2000, 2001 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the GNU Compiler Collection
#

# Defs for GCC
WARNINGS = -pedantic -Wall -Wundef -Wpointer-arith \
          -Wcast-align -Wwrite-strings -Wstrict-prototypes \
          -Wmissing-prototypes -Wmissing-declarations -Wnested-externs \
          -Winline -Wno-unused -Wno-long-long -W -Wcast-qual -Wshadow

CROSS_CFLAGS += -D__UNIXLIB_INTERNALS $(WARNINGS) 

#CROSS_CFLAGS += -D__UNIXLIB_INTERNALS -pedantic -Wall -Wno-long-long
INCLUDES = -isystem clib

libunixobj=$(objdir)/unixlib/$(TRG)

$(libunixobj)/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

$(libunixobj)/%.o: %.s
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(CFL) $(CFL1)

COMPLEX = $(libunixobj)/complex/cabs.o \
	$(libunixobj)/complex/cacos.o \
	$(libunixobj)/complex/cacosh.o \
	$(libunixobj)/complex/carg.o \
	$(libunixobj)/complex/casin.o \
	$(libunixobj)/complex/casinh.o \
	$(libunixobj)/complex/catan.o \
	$(libunixobj)/complex/catanh.o \
	$(libunixobj)/complex/ccos.o \
	$(libunixobj)/complex/ccosh.o \
	$(libunixobj)/complex/cexp.o \
	$(libunixobj)/complex/cimag.o \
	$(libunixobj)/complex/clog.o \
	$(libunixobj)/complex/clog10.o \
	$(libunixobj)/complex/conj.o \
	$(libunixobj)/complex/cpow.o \
	$(libunixobj)/complex/cproj.o \
	$(libunixobj)/complex/creal.o \
	$(libunixobj)/complex/csin.o \
	$(libunixobj)/complex/csinh.o \
	$(libunixobj)/complex/csqrt.o \
	$(libunixobj)/complex/ctan.o \
	$(libunixobj)/complex/ctanh.o

CRYPT =	$(libunixobj)/crypt/crypt.o \
	$(libunixobj)/crypt/crypt-entry.o \
	$(libunixobj)/crypt/crypt_util.o \
	$(libunixobj)/crypt/md5.o \
	$(libunixobj)/crypt/md5-crypt.o \

ICONV =	$(libunixobj)/iconv/iconv.o

MATH =	$(libunixobj)/math/acosh.o \
	$(libunixobj)/math/asinh.o \
	$(libunixobj)/math/atanh.o \
	$(libunixobj)/math/cbrt.o \
	$(libunixobj)/math/drem.o \
	$(libunixobj)/math/erf.o \
	$(libunixobj)/math/expm1.o \
	$(libunixobj)/math/fpclassify.o \
	$(libunixobj)/math/fma.o \
	$(libunixobj)/math/fmax.o \
	$(libunixobj)/math/fmin.o \
	$(libunixobj)/math/fdim.o \
	$(libunixobj)/math/fmod.o \
	$(libunixobj)/math/fmodf.o \
	$(libunixobj)/math/frexp.o \
	$(libunixobj)/math/frexpf.o \
	$(libunixobj)/math/huge_val.o \
	$(libunixobj)/math/ilogb.o \
	$(libunixobj)/math/j0.o \
	$(libunixobj)/math/j1.o \
	$(libunixobj)/math/jn.o \
	$(libunixobj)/math/k_cos.o \
	$(libunixobj)/math/k_rem_pio2.o \
	$(libunixobj)/math/k_sin.o \
	$(libunixobj)/math/lrint.o \
	$(libunixobj)/math/llrint.o \
	$(libunixobj)/math/log2.o \
	$(libunixobj)/math/lgamma.o \
	$(libunixobj)/math/lgamma_r.o \
	$(libunixobj)/math/log1p.o \
	$(libunixobj)/math/logb.o \
	$(libunixobj)/math/modf.o \
	$(libunixobj)/math/modff.o \
	$(libunixobj)/math/nextafter.o \
	$(libunixobj)/math/nearbyint.o \
	$(libunixobj)/math/remainder.o \
	$(libunixobj)/math/rem_pio2.o \
	$(libunixobj)/math/remquo.o \
	$(libunixobj)/math/rint.o \
	$(libunixobj)/math/scalbn.o \
	$(libunixobj)/math/significan.o \
	$(libunixobj)/math/trunc.o \
	$(libunixobj)/math/exp.o \
	$(libunixobj)/math/exp2.o \
	$(libunixobj)/math/exp10.o \
	$(libunixobj)/math/t_exp.o \
	$(libunixobj)/math/round.o \
	$(libunixobj)/math/lround.o \
	$(libunixobj)/math/llround.o

MATH_ASM = $(libunixobj)/math-asm/_acos.o \
	$(libunixobj)/math-asm/_acosf.o \
	$(libunixobj)/math-asm/_asin.o \
	$(libunixobj)/math-asm/_asinf.o \
	$(libunixobj)/math-asm/_atan.o \
	$(libunixobj)/math-asm/_atanf.o \
	$(libunixobj)/math-asm/_atan2.o \
	$(libunixobj)/math-asm/_atan2f.o \
	$(libunixobj)/math-asm/_ceil.o \
	$(libunixobj)/math-asm/_ceilf.o \
	$(libunixobj)/math-asm/_copysign.o \
	$(libunixobj)/math-asm/_cos.o \
	$(libunixobj)/math-asm/_cosf.o \
	$(libunixobj)/math-asm/_cosh.o \
	$(libunixobj)/math-asm/_coshf.o \
	$(libunixobj)/math-asm/_exp.o \
	$(libunixobj)/math-asm/_expf.o \
	$(libunixobj)/math-asm/_fabs.o \
	$(libunixobj)/math-asm/_finite.o \
	$(libunixobj)/math-asm/_floor.o \
	$(libunixobj)/math-asm/_floorf.o \
	$(libunixobj)/math-asm/_fclrexcpt.o \
	$(libunixobj)/math-asm/_fegetenv.o \
	$(libunixobj)/math-asm/_fegetrnd.o \
	$(libunixobj)/math-asm/_feholdexc.o \
	$(libunixobj)/math-asm/_fesetenv.o \
	$(libunixobj)/math-asm/_fesetroun.o \
	$(libunixobj)/math-asm/_fraiseexc.o \
	$(libunixobj)/math-asm/_fsetexcpt.o \
	$(libunixobj)/math-asm/_ftstexcpt.o \
	$(libunixobj)/math-asm/_hypot.o \
	$(libunixobj)/math-asm/_isinf.o \
	$(libunixobj)/math-asm/_isnan.o \
	$(libunixobj)/math-asm/_ldexp.o \
	$(libunixobj)/math-asm/_ldexpf.o \
	$(libunixobj)/math-asm/_log.o \
	$(libunixobj)/math-asm/_logf.o \
	$(libunixobj)/math-asm/_log10.o \
	$(libunixobj)/math-asm/_log10f.o \
	$(libunixobj)/math-asm/_pow.o \
	$(libunixobj)/math-asm/_powf.o \
	$(libunixobj)/math-asm/_signbit.o \
	$(libunixobj)/math-asm/_sin.o \
	$(libunixobj)/math-asm/_sinf.o \
	$(libunixobj)/math-asm/_sinh.o \
	$(libunixobj)/math-asm/_sinhf.o \
	$(libunixobj)/math-asm/_sqrt.o \
	$(libunixobj)/math-asm/_tan.o \
	$(libunixobj)/math-asm/_tanf.o \
	$(libunixobj)/math-asm/_tanh.o \
	$(libunixobj)/math-asm/_tanhf.o \
	$(libunixobj)/math-asm/_sincos.o

OBJS =	$(libunixobj)/abort.o \
	$(libunixobj)/abs.o \
	$(libunixobj)/alloc.o \
	$(libunixobj)/assert.o \
	$(libunixobj)/atexit.o \
	$(libunixobj)/bsearch.o \
	$(libunixobj)/ctype.o \
	$(libunixobj)/ctypeproc.o \
	$(libunixobj)/div.o \
	$(libunixobj)/errno.o \
	$(libunixobj)/index.o \
	$(libunixobj)/memccpy.o \
	$(libunixobj)/memchr.o \
	$(libunixobj)/memcmp.o \
	$(libunixobj)/memcpy.o \
	$(libunixobj)/memset.o \
	$(libunixobj)/opensock.o \
	$(libunixobj)/psignal.o \
	$(libunixobj)/qsort.o \
	$(libunixobj)/stpcpy.o \
	$(libunixobj)/stpncpy.o \
	$(libunixobj)/strcat.o \
	$(libunixobj)/strchr.o \
	$(libunixobj)/strcmp.o \
	$(libunixobj)/strcpy.o \
	$(libunixobj)/strcspn.o \
	$(libunixobj)/strdup.o \
	$(libunixobj)/strichr.o \
	$(libunixobj)/stricmp.o \
	$(libunixobj)/strlen.o \
	$(libunixobj)/strncat.o \
	$(libunixobj)/strncmp.o \
	$(libunixobj)/strncpy.o \
	$(libunixobj)/strnicmp.o \
	$(libunixobj)/strpbrk.o \
	$(libunixobj)/strrchr.o \
	$(libunixobj)/strrichr.o \
	$(libunixobj)/strsep.o \
	$(libunixobj)/strsignal.o \
	$(libunixobj)/strspn.o \
	$(libunixobj)/strstr.o \
	$(libunixobj)/strtod.o \
	$(libunixobj)/strtok.o \
	$(libunixobj)/strtok_r.o \
	$(libunixobj)/strtol.o \
	$(libunixobj)/strtoll.o \
	$(libunixobj)/strtoul.o \
	$(libunixobj)/strtoull.o \
	$(libunixobj)/system.o \
	$(libunixobj)/termcap.o

COMMON = $(libunixobj)/common/attrib.o \
	$(libunixobj)/common/cvttime.o \
	$(libunixobj)/common/exist.o \
	$(libunixobj)/common/fdname.o \
	$(libunixobj)/common/funcallerr.o \
	$(libunixobj)/common/riscosify.o \
	$(libunixobj)/common/riscosifyc.o \
	$(libunixobj)/common/serial.o \
	$(libunixobj)/common/sockvalid.o \
	$(libunixobj)/common/stat.o \
	$(libunixobj)/common/uname.o \
	$(libunixobj)/common/unixify.o

COMMON_ASM =	$(libunixobj)/common/_exist.o \
	$(libunixobj)/common/_riscosify.o

GCCCOMPAT_ASM = $(libunixobj)/gcccompat/_udivdi3.o \
	$(libunixobj)/gcccompat/_umoddi3.o \
	$(libunixobj)/gcccompat/_divdi3.o \
	$(libunixobj)/gcccompat/_moddi3.o

LOCALE = $(libunixobj)/locale/ctypetable.o \
	$(libunixobj)/locale/localeconv.o \
	$(libunixobj)/locale/setlocale.o \
	$(libunixobj)/locale/strcoll.o \
	$(libunixobj)/locale/strxfrm.o \
	$(libunixobj)/locale/territory.o

MODULE = $(libunixobj)/module/sul.o

NETLIB = $(libunixobj)/netlib/accept.o \
	$(libunixobj)/netlib/bind.o \
	$(libunixobj)/netlib/connect.o \
	$(libunixobj)/netlib/getpeernam.o \
	$(libunixobj)/netlib/getsocknam.o \
	$(libunixobj)/netlib/getsockopt.o \
	$(libunixobj)/netlib/host.o \
	$(libunixobj)/netlib/h_errno.o \
	$(libunixobj)/netlib/in6_addr.o \
	$(libunixobj)/netlib/inet_lnaof.o \
	$(libunixobj)/netlib/inet_mkad.o \
	$(libunixobj)/netlib/inet_netof.o \
	$(libunixobj)/netlib/inet_netwo.o \
	$(libunixobj)/netlib/inet_ntoa.o \
	$(libunixobj)/netlib/ntop.o \
	$(libunixobj)/netlib/listen.o \
	$(libunixobj)/netlib/net.o \
	$(libunixobj)/netlib/proto.o \
	$(libunixobj)/netlib/pton.o \
	$(libunixobj)/netlib/readline.o \
	$(libunixobj)/netlib/recv.o \
	$(libunixobj)/netlib/recvfrom.o \
	$(libunixobj)/netlib/recvmsg.o \
	$(libunixobj)/netlib/send.o \
	$(libunixobj)/netlib/sendmsg.o \
	$(libunixobj)/netlib/sendto.o \
	$(libunixobj)/netlib/serv.o \
	$(libunixobj)/netlib/setsockopt.o \
	$(libunixobj)/netlib/shutdown.o \
	$(libunixobj)/netlib/socket.o

NETLIB_ASM = \
	$(libunixobj)/netlib/_accept.o \
	$(libunixobj)/netlib/_bind.o \
	$(libunixobj)/netlib/_bswap_16.o \
	$(libunixobj)/netlib/_bswap_32.o \
	$(libunixobj)/netlib/_connect.o \
	$(libunixobj)/netlib/_getpeerna.o \
	$(libunixobj)/netlib/_getsockna.o \
	$(libunixobj)/netlib/_getsockop.o \
	$(libunixobj)/netlib/_gettsize.o \
	$(libunixobj)/netlib/_hostbyadd.o \
	$(libunixobj)/netlib/_hostbynam.o \
	$(libunixobj)/netlib/_listen.o \
	$(libunixobj)/netlib/_net_error.o \
	$(libunixobj)/netlib/_readv.o \
	$(libunixobj)/netlib/_recv.o \
	$(libunixobj)/netlib/_recvfrom.o \
	$(libunixobj)/netlib/_recvmsg.o \
	$(libunixobj)/netlib/_sclose.o \
	$(libunixobj)/netlib/_select.o \
	$(libunixobj)/netlib/_send.o \
	$(libunixobj)/netlib/_sendmsg.o \
	$(libunixobj)/netlib/_sendto.o \
	$(libunixobj)/netlib/_sendtosm.o \
	$(libunixobj)/netlib/_setsockop.o \
	$(libunixobj)/netlib/_shutdown.o \
	$(libunixobj)/netlib/_sioctl.o \
	$(libunixobj)/netlib/_socket.o \
	$(libunixobj)/netlib/_sockstat.o \
	$(libunixobj)/netlib/_sread.o \
	$(libunixobj)/netlib/_swrite.o \
	$(libunixobj)/netlib/_writev.o

PTHREAD = $(libunixobj)/pthread/atfork.o \
	$(libunixobj)/pthread/attr.o \
	$(libunixobj)/pthread/cancel.o \
	$(libunixobj)/pthread/cleanup.o \
	$(libunixobj)/pthread/cleanupcall.o \
	$(libunixobj)/pthread/cond.o \
	$(libunixobj)/pthread/condattr.o \
	$(libunixobj)/pthread/context.o \
	$(libunixobj)/pthread/create.o \
	$(libunixobj)/pthread/detach.o \
	$(libunixobj)/pthread/equal.o \
	$(libunixobj)/pthread/error.o \
	$(libunixobj)/pthread/exit.o \
	$(libunixobj)/pthread/init.o \
	$(libunixobj)/pthread/join.o \
	$(libunixobj)/pthread/key.o \
	$(libunixobj)/pthread/keydest.o \
	$(libunixobj)/pthread/lock.o \
	$(libunixobj)/pthread/mutex.o \
	$(libunixobj)/pthread/mutexattr.o \
	$(libunixobj)/pthread/newnode.o \
	$(libunixobj)/pthread/once.o \
	$(libunixobj)/pthread/rwlock.o \
	$(libunixobj)/pthread/rwlockattr.o \
	$(libunixobj)/pthread/self.o \
	$(libunixobj)/pthread/signal.o \
	$(libunixobj)/pthread/testcancel.o 

PTHREAD_ASM = $(libunixobj)/pthread/_context.o \
	$(libunixobj)/pthread/_exit.o \
	$(libunixobj)/pthread/_ints.o \
	$(libunixobj)/pthread/_yield.o 

PWD =	$(libunixobj)/pwd/fgetpwent.o \
	$(libunixobj)/pwd/fgetpwentr.o \
	$(libunixobj)/pwd/getlogin.o \
	$(libunixobj)/pwd/getpw.o \
	$(libunixobj)/pwd/getpwent.o \
	$(libunixobj)/pwd/getpwnam.o \
	$(libunixobj)/pwd/getpwnam_r.o \
	$(libunixobj)/pwd/getpwuid.o \
	$(libunixobj)/pwd/getpwuid_r.o \
	$(libunixobj)/pwd/putpwent.o \
	$(libunixobj)/pwd/pwdread.o

GRP =	$(libunixobj)/grp/fgetgrent.o \
	$(libunixobj)/grp/fgetgrentr.o \
	$(libunixobj)/grp/getgroups.o \
	$(libunixobj)/grp/getgrent.o \
	$(libunixobj)/grp/getgrnam.o \
	$(libunixobj)/grp/getgrnam_r.o \
	$(libunixobj)/grp/getgrgid.o \
	$(libunixobj)/grp/getgrgid_r.o \
	$(libunixobj)/grp/grpread.o

RESOLV = \
	$(libunixobj)/resolv/base64.o \
	$(libunixobj)/resolv/gethnamaddr.o \
	$(libunixobj)/resolv/herror.o \
	$(libunixobj)/resolv/inet_addr.o \
	$(libunixobj)/resolv/inet_neta.o \
	$(libunixobj)/resolv/inet_net_ntop.o \
	$(libunixobj)/resolv/inet_net_pton.o \
	$(libunixobj)/resolv/inet_ntop.o \
	$(libunixobj)/resolv/inet_pton.o \
	$(libunixobj)/resolv/nsap_addr.o \
	$(libunixobj)/resolv/ns_name.o \
	$(libunixobj)/resolv/ns_netint.o \
	$(libunixobj)/resolv/ns_parse.o \
	$(libunixobj)/resolv/ns_print.o \
	$(libunixobj)/resolv/ns_samedomain.o \
	$(libunixobj)/resolv/ns_ttl.o \
	$(libunixobj)/resolv/res_comp.o \
	$(libunixobj)/resolv/res_data.o \
	$(libunixobj)/resolv/res_debug.o \
	$(libunixobj)/resolv/res_hconf.o \
	$(libunixobj)/resolv/res_init.o \
	$(libunixobj)/resolv/res_mkquery.o \
	$(libunixobj)/resolv/res_query.o \
	$(libunixobj)/resolv/res_send.o

RESOURCE =	$(libunixobj)/resource/getegid.o \
	$(libunixobj)/resource/geteuid.o \
	$(libunixobj)/resource/getgid.o \
	$(libunixobj)/resource/getpgrp.o \
	$(libunixobj)/resource/getpid.o \
	$(libunixobj)/resource/getppid.o \
	$(libunixobj)/resource/getprior.o \
	$(libunixobj)/resource/getrlimit.o \
	$(libunixobj)/resource/getrusage.o \
	$(libunixobj)/resource/getuid.o \
	$(libunixobj)/resource/initialise.o \
	$(libunixobj)/resource/seteuid.o \
	$(libunixobj)/resource/setgid.o \
	$(libunixobj)/resource/setpgid.o \
	$(libunixobj)/resource/setpgrp.o \
	$(libunixobj)/resource/setprior.o \
	$(libunixobj)/resource/setrlimit.o \
	$(libunixobj)/resource/setuid.o \
	$(libunixobj)/resource/ulimit.o

SIGNAL =	$(libunixobj)/signal/init.o \
	$(libunixobj)/signal/kill.o \
	$(libunixobj)/signal/pause.o \
	$(libunixobj)/signal/post.o \
	$(libunixobj)/signal/raise.o \
	$(libunixobj)/signal/sigaction.o \
	$(libunixobj)/signal/sigaddset.o \
	$(libunixobj)/signal/sigaltstk.o \
	$(libunixobj)/signal/sigblock.o \
	$(libunixobj)/signal/sigdelset.o \
	$(libunixobj)/signal/siginfo.o \
	$(libunixobj)/signal/sigismembe.o \
	$(libunixobj)/signal/signal.o \
	$(libunixobj)/signal/signame.o \
	$(libunixobj)/signal/sigpause.o \
	$(libunixobj)/signal/sigpending.o \
	$(libunixobj)/signal/sigprocmsk.o \
	$(libunixobj)/signal/sigsetmask.o \
	$(libunixobj)/signal/sigsets.o \
	$(libunixobj)/signal/sigstack.o \
	$(libunixobj)/signal/sigsuspend.o \
	$(libunixobj)/signal/sleep.o

SIGNAL_ASM =	$(libunixobj)/signal/sigexec.o \
	$(libunixobj)/signal/_coredump.o \
	$(libunixobj)/signal/_signal.o

STDIO = $(libunixobj)/stdio/clearerr.o \
	$(libunixobj)/stdio/fclose.o \
	$(libunixobj)/stdio/feof.o \
	$(libunixobj)/stdio/ferror.o \
	$(libunixobj)/stdio/fflush.o \
	$(libunixobj)/stdio/fgets.o \
	$(libunixobj)/stdio/filbuf.o \
	$(libunixobj)/stdio/fileno.o \
	$(libunixobj)/stdio/flsbuf.o \
	$(libunixobj)/stdio/fopen.o \
	$(libunixobj)/stdio/fpos.o \
	$(libunixobj)/stdio/fputs.o \
	$(libunixobj)/stdio/fread.o \
	$(libunixobj)/stdio/freopen.o \
	$(libunixobj)/stdio/fwrite.o \
	$(libunixobj)/stdio/getc.o \
	$(libunixobj)/stdio/getdelim.o \
	$(libunixobj)/stdio/gets.o \
	$(libunixobj)/stdio/getw.o \
	$(libunixobj)/stdio/newstream.o \
	$(libunixobj)/stdio/perror.o \
	$(libunixobj)/stdio/popen.o \
	$(libunixobj)/stdio/printf.o \
	$(libunixobj)/stdio/putc.o \
	$(libunixobj)/stdio/puts.o \
	$(libunixobj)/stdio/putw.o \
	$(libunixobj)/stdio/remove.o \
	$(libunixobj)/stdio/rename.o \
	$(libunixobj)/stdio/scanf.o \
	$(libunixobj)/stdio/setbuf.o \
	$(libunixobj)/stdio/stdio.o \
	$(libunixobj)/stdio/tmpfile.o \
	$(libunixobj)/stdio/ungetc.o

STDIO_ASM =	$(libunixobj)/stdio/_printf.o

STDLIB = $(libunixobj)/stdlib/drand48.o \
	$(libunixobj)/stdlib/erand48.o \
	$(libunixobj)/stdlib/jrand48.o \
	$(libunixobj)/stdlib/lcong48.o \
	$(libunixobj)/stdlib/mrand48.o \
	$(libunixobj)/stdlib/nrand48.o \
	$(libunixobj)/stdlib/_rand48.o \
	$(libunixobj)/stdlib/rand.o \
	$(libunixobj)/stdlib/random.o \
	$(libunixobj)/stdlib/realpath.o \
	$(libunixobj)/stdlib/seed48.o \
	$(libunixobj)/stdlib/srand48.o \
	$(libunixobj)/stdlib/strtof.o \
	$(libunixobj)/stdlib/strtold.o

STRING = \
	$(libunixobj)/string/rawmemchr.o \
	$(libunixobj)/string/strchrnul.o \
	$(libunixobj)/string/strndup.o \
	$(libunixobj)/string/strnlen.o \
	$(libunixobj)/string/swab.o

SCL_ASM = $(libunixobj)/scl/_kcurskch.o \
	$(libunixobj)/scl/_kernel.o \
	$(libunixobj)/scl/_kgetenv.o \
	$(libunixobj)/scl/_kosargs.o \
	$(libunixobj)/scl/_kosbget.o \
	$(libunixobj)/scl/_kosbput.o \
	$(libunixobj)/scl/_kosbyte.o \
	$(libunixobj)/scl/_koscli.o \
	$(libunixobj)/scl/_kosfile.o \
	$(libunixobj)/scl/_kosfind.o \
	$(libunixobj)/scl/_kosgbpb.o \
	$(libunixobj)/scl/_kosrdch.o \
	$(libunixobj)/scl/_kosword.o \
	$(libunixobj)/scl/_koswrch.o \
	$(libunixobj)/scl/_ksetenv.o \
	$(libunixobj)/scl/_kswi_c.o

STAT =	$(libunixobj)/stat/mknod.o \
	$(libunixobj)/stat/mkfifo.o

SYS =	$(libunixobj)/sys/alarm.o \
	$(libunixobj)/sys/brk.o \
	$(libunixobj)/sys/debug.o \
	$(libunixobj)/sys/errlist.o \
	$(libunixobj)/sys/exec.o \
	$(libunixobj)/sys/execl.o \
	$(libunixobj)/sys/execle.o \
	$(libunixobj)/sys/execlp.o \
	$(libunixobj)/sys/execv.o \
	$(libunixobj)/sys/execvp.o \
	$(libunixobj)/sys/mman.o \
	$(libunixobj)/sys/sigsetjmp.o \
	$(libunixobj)/sys/stackalloc.o \
	$(libunixobj)/sys/vfork.o

SYS_ASM =	$(libunixobj)/sys/_alloca.o \
	$(libunixobj)/sys/_divsi3.o \
	$(libunixobj)/sys/_divtest.o \
	$(libunixobj)/sys/_exec.o \
	$(libunixobj)/sys/_jmp.o \
	$(libunixobj)/sys/_mem.o \
	$(libunixobj)/sys/_mman.o \
	$(libunixobj)/sys/_modsi3.o \
	$(libunixobj)/sys/_os.o \
	$(libunixobj)/sys/_profile.o \
	$(libunixobj)/sys/_sigjmp.o \
	$(libunixobj)/sys/_sdiv.o \
	$(libunixobj)/sys/_sdiv10.o \
	$(libunixobj)/sys/_smod.o \
	$(libunixobj)/sys/_swi.o \
	$(libunixobj)/sys/_syslib.o \
	$(libunixobj)/sys/_udiv.o \
	$(libunixobj)/sys/_udiv10.o \
	$(libunixobj)/sys/_udivsi3.o \
	$(libunixobj)/sys/_umod.o \
	$(libunixobj)/sys/_umodsi3.o \
	$(libunixobj)/sys/_vfork.o

TIME =	$(libunixobj)/time/adjtime.o \
	$(libunixobj)/time/asctime.o \
	$(libunixobj)/time/broken.o \
	$(libunixobj)/time/calendconv.o \
	$(libunixobj)/time/ctime.o \
	$(libunixobj)/time/difftime.o \
	$(libunixobj)/time/ftime.o \
	$(libunixobj)/time/getitmr.o \
	$(libunixobj)/time/gettod.o \
	$(libunixobj)/time/gmtime.o \
	$(libunixobj)/time/gmtime_r.o \
	$(libunixobj)/time/localtime.o \
	$(libunixobj)/time/localtime_r.o \
	$(libunixobj)/time/mktime.o \
	$(libunixobj)/time/setitmr.o \
	$(libunixobj)/time/settod.o \
	$(libunixobj)/time/stdtime.o \
	$(libunixobj)/time/strftime.o \
	$(libunixobj)/time/time.o \
	$(libunixobj)/time/times.o \
	$(libunixobj)/time/tzset.o \
	$(libunixobj)/time/utimes.o

TIME_ASM =	$(libunixobj)/time/_clock.o

TERMIOS =	$(libunixobj)/termios/cfgetispee.o \
	$(libunixobj)/termios/cfgetospee.o \
	$(libunixobj)/termios/cfsetispee.o \
	$(libunixobj)/termios/cfsetospee.o \
	$(libunixobj)/termios/ctermid.o \
	$(libunixobj)/termios/gtty.o \
	$(libunixobj)/termios/stty.o \
	$(libunixobj)/termios/tcdrain.o \
	$(libunixobj)/termios/tcflow.o \
	$(libunixobj)/termios/tcflush.o \
	$(libunixobj)/termios/tcgetattr.o \
	$(libunixobj)/termios/tcgetpgrp.o \
	$(libunixobj)/termios/tcsendbrk.o \
	$(libunixobj)/termios/tcsetattr.o \
	$(libunixobj)/termios/tcsetpgrp.o

UNISTD =	$(libunixobj)/unistd/chown.o \
	$(libunixobj)/unistd/chroot.o \
	$(libunixobj)/unistd/fchown.o \
	$(libunixobj)/unistd/getpass.o \
	$(libunixobj)/unistd/link.o \
	$(libunixobj)/unistd/readlink.o \
	$(libunixobj)/unistd/symlink.o

UNIX =	$(libunixobj)/unix/access.o \
	$(libunixobj)/unix/alphasort.o \
	$(libunixobj)/unix/chdir.o \
	$(libunixobj)/unix/chmod.o \
	$(libunixobj)/unix/close.o \
	$(libunixobj)/unix/creat.o \
	$(libunixobj)/unix/getdtables.o \
	$(libunixobj)/unix/dev.o \
	$(libunixobj)/unix/dirent.o \
	$(libunixobj)/unix/dup.o \
	$(libunixobj)/unix/dup2.o \
	$(libunixobj)/unix/fchmod.o \
	$(libunixobj)/unix/features.o \
	$(libunixobj)/unix/fnmatch.o \
	$(libunixobj)/unix/fcntl.o \
	$(libunixobj)/unix/fstat.o \
	$(libunixobj)/unix/getcwd.o \
	$(libunixobj)/unix/getdomain.o \
	$(libunixobj)/unix/getenv.o \
	$(libunixobj)/unix/gethostnam.o \
	$(libunixobj)/unix/getopt.o \
	$(libunixobj)/unix/getsubopt.o \
	$(libunixobj)/unix/glob.o \
	$(libunixobj)/unix/ioctl.o \
	$(libunixobj)/unix/isatty.o \
	$(libunixobj)/unix/ispipe.o \
	$(libunixobj)/unix/lseek.o \
	$(libunixobj)/unix/lstat.o \
	$(libunixobj)/unix/mkdir.o \
	$(libunixobj)/unix/nice.o \
	$(libunixobj)/unix/open.o \
	$(libunixobj)/unix/pathconf.o \
	$(libunixobj)/unix/pipe.o \
	$(libunixobj)/unix/poll.o \
	$(libunixobj)/unix/proc.o \
	$(libunixobj)/unix/pselect.o \
	$(libunixobj)/unix/putenv.o \
	$(libunixobj)/unix/read.o \
	$(libunixobj)/unix/readv.o \
	$(libunixobj)/unix/reopen.o \
	$(libunixobj)/unix/rmdir.o \
	$(libunixobj)/unix/select.o \
	$(libunixobj)/unix/setenv.o \
	$(libunixobj)/unix/stat.o \
	$(libunixobj)/unix/sync.o \
	$(libunixobj)/unix/sysconf.o \
	$(libunixobj)/unix/syslog.o \
	$(libunixobj)/unix/truncate.o \
	$(libunixobj)/unix/tty.o \
	$(libunixobj)/unix/umask.o \
	$(libunixobj)/unix/uname.o \
	$(libunixobj)/unix/unix.o \
	$(libunixobj)/unix/unlink.o \
	$(libunixobj)/unix/utime.o \
	$(libunixobj)/unix/wait.o \
	$(libunixobj)/unix/wait3.o \
	$(libunixobj)/unix/wait4.o \
	$(libunixobj)/unix/waitpid.o \
	$(libunixobj)/unix/write.o \
	$(libunixobj)/unix/writev.o

UNIX_ASM = $(libunixobj)/unix/getpagesiz.o

WCHAR = $(libunixobj)/wchar/btowc.o \
	$(libunixobj)/wchar/mblen.o \
	$(libunixobj)/wchar/mbrlen.o \
	$(libunixobj)/wchar/mbrtowc.o \
	$(libunixobj)/wchar/mbsinit.o \
	$(libunixobj)/wchar/mbsnrtowcs.o \
	$(libunixobj)/wchar/mbsrtowcs.o \
	$(libunixobj)/wchar/mbstowcs.o \
	$(libunixobj)/wchar/mbtowc.o \
	$(libunixobj)/wchar/wcpcpy.o \
	$(libunixobj)/wchar/wcpncpy.o \
	$(libunixobj)/wchar/wcrtomb.o \
	$(libunixobj)/wchar/wcscat.o \
	$(libunixobj)/wchar/wcschr.o \
	$(libunixobj)/wchar/wcscmp.o \
	$(libunixobj)/wchar/wcscoll.o \
	$(libunixobj)/wchar/wcscpy.o \
	$(libunixobj)/wchar/wcscspn.o \
	$(libunixobj)/wchar/wcsdup.o \
	$(libunixobj)/wchar/wcslen.o \
	$(libunixobj)/wchar/wcsncat.o \
	$(libunixobj)/wchar/wcsncmp.o \
	$(libunixobj)/wchar/wcsncpy.o \
	$(libunixobj)/wchar/wcsnrtombs.o \
	$(libunixobj)/wchar/wcspbrk.o \
	$(libunixobj)/wchar/wcsrchr.o \
	$(libunixobj)/wchar/wcsrtombs.o \
	$(libunixobj)/wchar/wcsspn.o \
	$(libunixobj)/wchar/wcsstr.o \
	$(libunixobj)/wchar/wcstok.o \
	$(libunixobj)/wchar/wcstombs.o \
	$(libunixobj)/wchar/wcswidth.o \
	$(libunixobj)/wchar/wcsxfrm.o \
	$(libunixobj)/wchar/wctob.o \
	$(libunixobj)/wchar/wctomb.o \
	$(libunixobj)/wchar/wcwidth.o \
	$(libunixobj)/wchar/wmemchr.o \
	$(libunixobj)/wchar/wmemcmp.o \
	$(libunixobj)/wchar/wmemcpy.o \
	$(libunixobj)/wchar/wmemmove.o \
	$(libunixobj)/wchar/wmemset.o

# wchar.o.wcstol wchar.o.wcstoul wchar.o.wcstoq wchar.o.wcstouq
#  wchar.o.wcstold wchar.o.wcstod wchar.o.wcstof

C_OBJS = $(COMPLEX) $(CRYPT) $(MATH) $(OBJS) $(COMMON) $(LOCALE) $(NETLIB) \
	$(PTHREAD) $(PWD) $(GRP) $(RESOLV) $(RESOURCE) $(SIGNAL) $(STAT) \
	$(STDIO) $(STRING) $(STDLIB) $(SYS) $(TIME) $(TERMIOS) $(UNISTD) \
	$(UNIX) $(WCHAR)

ASM_OBJS = $(MATH_ASM) $(COMMON_ASM) $(GCCCOMPAT_ASM) $(NETLIB_ASM) \
	$(PTHREAD_ASM) $(SIGNAL_ASM) $(STDIO_ASM) $(SCL_ASM) $(SYS_ASM) \
	$(TIME_ASM) $(UNIX_ASM)

SUL = $(ro_syspkg)/310/Modules/SharedULib,ffa

all:	$(ux_gcc_bin_dir)/$(TRG)/unixlib.o $(ro_gcc_bin_dir)/$(TRG)/o/unixlib \
	$(ro_unixlibpkg)/source/clib/o/unixlib $(SUL)

$(libunixobj)/sys/stackalloc.o: sys/stackalloc.c
	$(CROSS_CC) $(CROSS_CFLAGS) -mno-apcs-stack-check -o $@ -c $< $(INCLUDES) $(CFL)

install_headers:
	$(insthdr) clib "*.h" unixlib $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/arpa "*.h" unixlib/arpa $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/bits "*.h" unixlib/bits $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/machine "*.h" unixlib/machine $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/net "*.h" unixlib/net $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/netinet "*.h" unixlib/netinet $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/resolv "*.h" unixlib/resolv $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/rpc "*.h" unixlib/rpc $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/stdio-common "*.h" unixlib/stdio-common $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/string "*.h" unixlib/string $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/sys "*.h" unixlib/sys $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) clib/unixlib "*.[hs]" unixlib/unixlib $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)

install_source:
	$(instsrc) . "*.[chs]" source $(ro_unixlibpkg)
	for dir in common complex crypt gcccompat grp iconv locale math math-asm \
		module netlib pwd resolv resource scl signal stat stdio stdlib sys \
		 string termios time unistd unix wchar clib clib/arpa \
		 clib/machine clib/net clib/netinet clib/sys clib/unixlib; do \
	  $(instsrc) $$dir "*.[chs]" source/$$dir $(ro_unixlibpkg) ; done

# Split AR to work around Cygwin 16Kbyte command line limit,
$(ux_gcc_bin_dir)/$(TRG)/unixlib.o $(ro_gcc_bin_dir)/$(TRG)/o/unixlib $(ro_unixlibpkg)/source/clib/o/unixlib: $(C_OBJS) $(ASM_OBJS)
	$(CROSS_AR) -c $@ $(COMPLEX)
	$(CROSS_AR) -i $@ $(CRYPT)
	$(CROSS_AR) -i $@ $(MATH)
	$(CROSS_AR) -i $@ $(OBJS)
	$(CROSS_AR) -i $@ $(COMMON)
	#$(CROSS_AR) -i $@ $(ICONV)
	$(CROSS_AR) -i $@ $(LOCALE)
	$(CROSS_AR) -i $@ $(NETLIB)
	$(CROSS_AR) -i $@ $(PTHREAD)
	$(CROSS_AR) -i $@ $(PWD)
	$(CROSS_AR) -i $@ $(GRP)
	$(CROSS_AR) -i $@ $(RESOLV)
	$(CROSS_AR) -i $@ $(RESOURCE)
	$(CROSS_AR) -i $@ $(SIGNAL)
	$(CROSS_AR) -i $@ $(STAT)
	$(CROSS_AR) -i $@ $(STDIO)
	$(CROSS_AR) -i $@ $(STDLIB)
	$(CROSS_AR) -i $@ $(STRING)
	$(CROSS_AR) -i $@ $(SYS)
	$(CROSS_AR) -i $@ $(TIME)
	$(CROSS_AR) -i $@ $(TERMIOS)
	$(CROSS_AR) -i $@ $(UNISTD)
	$(CROSS_AR) -i $@ $(UNIX)
	$(CROSS_AR) -i $@ $(WCHAR)
	$(CROSS_AR) -i $@ $(MATH_ASM)
	$(CROSS_AR) -i $@ $(COMMON_ASM)
	$(CROSS_AR) -i $@ $(GCCCOMPAT_ASM)
	$(CROSS_AR) -i $@ $(NETLIB_ASM)
	$(CROSS_AR) -i $@ $(PTHREAD_ASM)
	$(CROSS_AR) -i $@ $(SIGNAL_ASM)
	$(CROSS_AR) -i $@ $(STDIO_ASM)
	$(CROSS_AR) -i $@ $(SCL_ASM)
	$(CROSS_AR) -i $@ $(SYS_ASM)
	$(CROSS_AR) -i $@ $(TIME_ASM)
	$(CROSS_AR) -i $@ $(UNIX_ASM)

$(SUL):	$(MODULE)
	$(CROSS_CC) $(MODULE) -Xlinker -module -o $@

# Static dependencies:
$(libunixobj)/abort.o: abort.c
$(libunixobj)/abs.o: abs.c
$(libunixobj)/alloc.o: alloc.c
$(libunixobj)/assert.o: assert.c
$(libunixobj)/atexit.o: atexit.c
$(libunixobj)/bsearch.o: bsearch.c
$(libunixobj)/ctype.o: ctype.c
$(libunixobj)/ctypeproc.o: ctypeproc.c
$(libunixobj)/div.o: div.c
$(libunixobj)/dtoa.o: dtoa.c
$(libunixobj)/errno.o: errno.c
$(libunixobj)/index.o: index.c
$(libunixobj)/memchr.o: memchr.c
$(libunixobj)/memcmp.o: memcmp.c
$(libunixobj)/memcpy.o: memcpy.c
$(libunixobj)/memccpy.o: memccpy.c
$(libunixobj)/memset.o: memset.c
$(libunixobj)/opensock.o: opensock.c
$(libunixobj)/psignal.o: psignal.c
$(libunixobj)/qsort.o: qsort.c
$(libunixobj)/stpcpy.o: stpcpy.c
$(libunixobj)/stpncpy.o: stpncpy.c
$(libunixobj)/strcat.o: strcat.c
$(libunixobj)/strchr.o: strchr.c
$(libunixobj)/strcmp.o: strcmp.c
$(libunixobj)/strcpy.o: strcpy.c
$(libunixobj)/strcspn.o: strcspn.c
$(libunixobj)/strdup.o: strdup.c
$(libunixobj)/strichr.o: strichr.c
$(libunixobj)/stricmp.o: stricmp.c
$(libunixobj)/strlen.o: strlen.c
$(libunixobj)/strncat.o: strncat.c
$(libunixobj)/strncmp.o: strncmp.c
$(libunixobj)/strncpy.o: strncpy.c
$(libunixobj)/strnicmp.o: strnicmp.c
$(libunixobj)/strpbrk.o: strpbrk.c
$(libunixobj)/strrchr.o: strrchr.c
$(libunixobj)/strrichr.o: strrichr.c
$(libunixobj)/strsep.o: strsep.c
$(libunixobj)/strsignal.o: strsignal.c
$(libunixobj)/strspn.o: strspn.c
$(libunixobj)/strstr.o: strstr.c
$(libunixobj)/strtod.o: strtod.c
$(libunixobj)/strtok.o: strtok.c
$(libunixobj)/strtok_r.o: strtok_r.c
$(libunixobj)/strtol.o: strtol.c
$(libunixobj)/strtoll.o: strtoll.c
$(libunixobj)/strtoul.o: strtoul.c
$(libunixobj)/strtoull.o: strtoull.c
$(libunixobj)/system.o: system.c
$(libunixobj)/termcap.o: termcap.c

$(libunixobj)/common/attrib.o: common/attrib.c
$(libunixobj)/common/cvttime.o: common/cvttime.c
$(libunixobj)/common/exist.o: common/exist.c
$(libunixobj)/common/fdname.o: common/fdname.c
$(libunixobj)/common/funcallerr.o: common/funcallerr.c
$(libunixobj)/common/riscosify.o: common/riscosify.c
$(libunixobj)/common/riscosifyc.o: common/riscosifyc.c
$(libunixobj)/common/serial.o: common/serial.c
$(libunixobj)/common/sockvalid.o: common/sockvalid.c
$(libunixobj)/common/stat.o: common/stat.c
$(libunixobj)/common/uname.o: common/uname.c
$(libunixobj)/common/unixify.o: common/unixify.c
$(libunixobj)/common/_exist.o: common/_exist.s
$(libunixobj)/common/_riscosify.o: common/_riscosify.s

$(libunixobj)/complex/cabs.o: complex/cabs.c
$(libunixobj)/complex/cacos.o: complex/cacos.c
$(libunixobj)/complex/cacosh.o: complex/cacosh.c
$(libunixobj)/complex/carg.o: complex/carg.c
$(libunixobj)/complex/casin.o: complex/casin.c
$(libunixobj)/complex/casinh.o: complex/casinh.c
$(libunixobj)/complex/catan.o: complex/catan.c
$(libunixobj)/complex/catanh.o: complex/catanh.c
$(libunixobj)/complex/ccos.o: complex/ccos.c
$(libunixobj)/complex/ccosh.o: complex/ccosh.c
$(libunixobj)/complex/cexp.o: complex/cexp.c
$(libunixobj)/complex/cimag.o: complex/cimag.c
$(libunixobj)/complex/clog.o: complex/clog.c
$(libunixobj)/complex/clog10.o: complex/clog10.c
$(libunixobj)/complex/conj.o: complex/conj.c
$(libunixobj)/complex/cpow.o: complex/cpow.c
$(libunixobj)/complex/cproj.o: complex/cproj.c
$(libunixobj)/complex/creal.o: complex/creal.c
$(libunixobj)/complex/csin.o: complex/csin.c
$(libunixobj)/complex/csinh.o: complex/csinh.c
$(libunixobj)/complex/csqrt.o: complex/csqrt.c
$(libunixobj)/complex/ctan.o: complex/ctan.c
$(libunixobj)/complex/ctanh.o: complex/ctanh.c

$(libunixobj)/crypt/crypt.o: crypt/crypt.c
$(libunixobj)/crypt/crypt-entry.o: crypt/crypt-entry.c
$(libunixobj)/crypt/crypt_util.o: crypt/crypt_util.c
$(libunixobj)/crypt/md5.o: crypt/md5.c
$(libunixobj)/crypt/md5-crypt.o: crypt/md5-crypt.c

$(libunixobj)/gcccompat/_udivdi3.o: gcccompat/_udivdi3.s clib/unixlib/asm_dec.s
$(libunixobj)/gcccompat/_umoddi3.o: gcccompat/_umoddi3.s clib/unixlib/asm_dec.s
$(libunixobj)/gcccompat/_divdi3.o: gcccompat/_divdi3.s clib/unixlib/asm_dec.s
$(libunixobj)/gcccompat/_moddi3.o: gcccompat/_moddi3.s clib/unixlib/asm_dec.s

$(libunixobj)/iconv/iconv.o: iconv/iconv.c

$(libunixobj)/locale/ctypetable.o: locale/ctypetable.c
$(libunixobj)/locale/localeconv.o: locale/localeconv.c
$(libunixobj)/locale/setlocale.o: locale/setlocale.c
$(libunixobj)/locale/strcoll.o: locale/strcoll.c
$(libunixobj)/locale/strxfrm.o: locale/strxfrm.c
$(libunixobj)/locale/territory.o: locale/territory.c

$(libunixobj)/math/acosh.o: math/acosh.c
$(libunixobj)/math/asinh.o: math/asinh.c
$(libunixobj)/math/atanh.o: math/atanh.c
$(libunixobj)/math/cbrt.o: math/cbrt.c
$(libunixobj)/math/drem.o: math/drem.c
$(libunixobj)/math/erf.o: math/erf.c
$(libunixobj)/math/exp.o: math/exp.c
$(libunixobj)/math/exp2.o: math/exp2.c
$(libunixobj)/math/exp10.o: math/exp10.c
$(libunixobj)/math/expm1.o: math/expm1.c
$(libunixobj)/math/fpclassify.o: math/fpclassify.c
$(libunixobj)/math/fdim.o: math/fdim.c
$(libunixobj)/math/fmod.o: math/fmod.c
$(libunixobj)/math/fmodf.o: math/fmodf.c
$(libunixobj)/math/fma.o: math/fma.c
$(libunixobj)/math/fmax.o: math/fmax.c
$(libunixobj)/math/fmin.o: math/fmin.c
$(libunixobj)/math/frexp.o: math/frexp.c
$(libunixobj)/math/frexpf.o: math/frexpf.c
$(libunixobj)/math/huge_val.o: math/huge_val.c
$(libunixobj)/math/ilogb.o: math/ilogb.c
$(libunixobj)/math/j0.o: math/j0.c
$(libunixobj)/math/j1.o: math/j1.c
$(libunixobj)/math/jn.o: math/jn.c
$(libunixobj)/math/k_cos.o: math/k_cos.c
$(libunixobj)/math/k_rem_pio2.o: math/k_rem_pio2.c
$(libunixobj)/math/k_sin.o: math/k_sin.c
$(libunixobj)/math/lgamma.o: math/lgamma.c
$(libunixobj)/math/lgamma_r.o: math/lgamma_r.c
$(libunixobj)/math/llrint.o: math/llrint.c
$(libunixobj)/math/lrint.o: math/lrint.c
$(libunixobj)/math/lround.o: math/lround.c
$(libunixobj)/math/llround.o: math/llround.c
$(libunixobj)/math/log1p.o: math/log1p.c
$(libunixobj)/math/log2.o: math/log2.c
$(libunixobj)/math/logb.o: math/logb.c
$(libunixobj)/math/modf.o: math/modf.c
$(libunixobj)/math/modff.o: math/modff.c
$(libunixobj)/math/nearbyint.o: math/nearbyint.c
$(libunixobj)/math/nextafter.o: math/nextafter.c
$(libunixobj)/math/remainder.o: math/remainder.c
$(libunixobj)/math/rem_pio2.o: math/rem_pio2.c
$(libunixobj)/math/remquo.o: math/remquo.c
$(libunixobj)/math/round.o: math/round.c
$(libunixobj)/math/rint.o: math/rint.c
$(libunixobj)/math/scalbn.o: math/scalbn.c
$(libunixobj)/math/significan.o: math/significan.c
$(libunixobj)/math/trunc.o: math/trunc.c
$(libunixobj)/math/t_exp.o: math/t_exp.c

$(libunixobj)/math-asm/_acos.o: math-asm/_acos.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_acosf.o: math-asm/_acos.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_asin.o: math-asm/_asin.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_asinf.o: math-asm/_asin.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_atan.o: math-asm/_atan.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_atanf.o: math-asm/_atan.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_atan2.o: math-asm/_atan2.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_atan2f.o: math-asm/_atan2.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_ceil.o: math-asm/_ceil.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_ceilf.o: math-asm/_ceil.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_copysign.o: math-asm/_copysign.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_cos.o: math-asm/_cos.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_cosf.o: math-asm/_cos.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_cosh.o: math-asm/_cosh.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_coshf.o: math-asm/_cosh.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_exp.o: math-asm/_exp.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_expf.o: math-asm/_exp.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_fabs.o: math-asm/_fabs.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_finite.o: math-asm/_finite.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_floor.o: math-asm/_floor.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_floorf.o: math-asm/_floor.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_fclrexcpt.o: math-asm/_fclrexcpt.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_fegetenv.o: math-asm/_fegetenv.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_fegetrnd.o: math-asm/_fegetrnd.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_feholdexc.o: math-asm/_feholdexc.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_fesetenv.o: math-asm/_fesetenv.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_fesetroun.o: math-asm/_fesetroun.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_fraiseexc.o: math-asm/_fraiseexc.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_fsetexcpt.o: math-asm/_fsetexcpt.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_ftstexcpt.o: math-asm/_ftstexcpt.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_hypot.o: math-asm/_hypot.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_isinf.o: math-asm/_isinf.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_isnan.o: math-asm/_isnan.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_ldexp.o: math-asm/_ldexp.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_ldexpf.o: math-asm/_ldexp.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_log.o: math-asm/_log.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_logf.o: math-asm/_log.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_log10.o: math-asm/_log10.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_log10f.o: math-asm/_log10.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_modf1.o: math-asm/_modf1.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_modf1f.o: math-asm/_modf1.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_pow.o: math-asm/_pow.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_powf.o: math-asm/_pow.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_signbit.o: math-asm/_signbit.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_sin.o: math-asm/_sin.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_sinf.o: math-asm/_sin.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_sincos.o: math-asm/_sincos.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_sinh.o: math-asm/_sinh.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_sinhf.o: math-asm/_sinh.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_sqrt.o: math-asm/_sqrt.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_tan.o: math-asm/_tan.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_tanf.o: math-asm/_tan.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_tanh.o: math-asm/_tanh.s clib/unixlib/asm_dec.s
$(libunixobj)/math-asm/_tanhf.o: math-asm/_tanh.s clib/unixlib/asm_dec.s

$(libunixobj)/module/sul.o: module/sul.s

$(libunixobj)/netlib/accept.o: netlib/accept.c
$(libunixobj)/netlib/bind.o: netlib/bind.c
$(libunixobj)/netlib/connect.o: netlib/connect.c
$(libunixobj)/netlib/getpeernam.o: netlib/getpeernam.c
$(libunixobj)/netlib/getsocknam.o: netlib/getsocknam.c
$(libunixobj)/netlib/getsockopt.o: netlib/getsockopt.c
$(libunixobj)/netlib/host.o: netlib/host.c
$(libunixobj)/netlib/h_errno.o: netlib/h_errno.c
$(libunixobj)/netlib/in6_addr.o: netlib/in6_addr.c
$(libunixobj)/netlib/inet_lnaof.o: netlib/inet_lnaof.c
$(libunixobj)/netlib/inet_mkad.o: netlib/inet_mkad.c
$(libunixobj)/netlib/inet_netof.o: netlib/inet_netof.c
$(libunixobj)/netlib/inet_netwo.o: netlib/inet_netwo.c
$(libunixobj)/netlib/inet_ntoa.o: netlib/inet_ntoa.c
$(libunixobj)/netlib/listen.o: netlib/listen.c
$(libunixobj)/netlib/net.o: netlib/net.c
$(libunixobj)/netlib/ntop.o: netlib/ntop.c
$(libunixobj)/netlib/proto.o: netlib/proto.c
$(libunixobj)/netlib/pton.o: netlib/pton.c
$(libunixobj)/netlib/readline.o: netlib/readline.c
$(libunixobj)/netlib/recv.o: netlib/recv.c
$(libunixobj)/netlib/recvfrom.o: netlib/recvfrom.c
$(libunixobj)/netlib/recvmsg.o: netlib/recvmsg.c
$(libunixobj)/netlib/send.o: netlib/send.c
$(libunixobj)/netlib/sendmsg.o: netlib/sendmsg.c
$(libunixobj)/netlib/sendto.o: netlib/sendto.c
$(libunixobj)/netlib/serv.o: netlib/serv.c
$(libunixobj)/netlib/setsockopt.o: netlib/setsockopt.c
$(libunixobj)/netlib/shutdown.o: netlib/shutdown.c
$(libunixobj)/netlib/socket.o: netlib/socket.c
$(libunixobj)/netlib/_accept.o: netlib/_accept.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_bind.o: netlib/_bind.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_bswap_16.o: netlib/_bswap_16.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_bswap_32.o: netlib/_bswap_32.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_connect.o: netlib/_connect.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_getpeerna.o: netlib/_getpeerna.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_getsockna.o: netlib/_getsockna.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_getsockop.o: netlib/_getsockop.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_gettsize.o: netlib/_gettsize.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_hostbyadd.o: netlib/_hostbyadd.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_hostbynam.o: netlib/_hostbynam.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_listen.o: netlib/_listen.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_net_error.o: netlib/_net_error.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_readv.o: netlib/_readv.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_recv.o: netlib/_recv.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_recvfrom.o: netlib/_recvfrom.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_recvmsg.o: netlib/_recvmsg.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_sclose.o: netlib/_sclose.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_select.o: netlib/_select.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_send.o: netlib/_send.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_sendmsg.o: netlib/_sendmsg.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_sendto.o: netlib/_sendto.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_sendtosm.o: netlib/_sendtosm.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_setsockop.o: netlib/_setsockop.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_shutdown.o: netlib/_shutdown.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_sioctl.o: netlib/_sioctl.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_socket.o: netlib/_socket.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_sockstat.o: netlib/_sockstat.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_sread.o: netlib/_sread.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_swrite.o: netlib/_swrite.s clib/unixlib/asm_dec.s
$(libunixobj)/netlib/_writev.o: netlib/_writev.s clib/unixlib/asm_dec.s

$(libunixobj)/pthread/atfork.o: pthread/atfork.c clib/pthread.h
$(libunixobj)/pthread/attr.o: pthread/attr.c clib/pthread.h
$(libunixobj)/pthread/cancel.o: pthread/cancel.c clib/pthread.h
$(libunixobj)/pthread/cleanup.o: pthread/cleanup.c clib/pthread.h
$(libunixobj)/pthread/cleanupcall.o: pthread/cleanupcall.c clib/pthread.h
$(libunixobj)/pthread/cond.o: pthread/cond.c clib/pthread.h
$(libunixobj)/pthread/condattr.o: pthread/condattr.c clib/pthread.h
$(libunixobj)/pthread/context.o: pthread/context.c clib/pthread.h
$(libunixobj)/pthread/create.o: pthread/create.c clib/pthread.h
$(libunixobj)/pthread/detach.o: pthread/detach.c clib/pthread.h
$(libunixobj)/pthread/equal.o: pthread/equal.c clib/pthread.h
$(libunixobj)/pthread/error.o: pthread/error.c clib/pthread.h
$(libunixobj)/pthread/exit.o: pthread/exit.c clib/pthread.h
$(libunixobj)/pthread/init.o: pthread/init.c clib/pthread.h
$(libunixobj)/pthread/join.o: pthread/join.c clib/pthread.h
$(libunixobj)/pthread/key.o: pthread/key.c clib/pthread.h
$(libunixobj)/pthread/keydest.o: pthread/keydest.c clib/pthread.h
$(libunixobj)/pthread/lock.o: pthread/lock.c clib/pthread.h
$(libunixobj)/pthread/mutex.o: pthread/mutex.c clib/pthread.h
$(libunixobj)/pthread/mutexattr.o: pthread/mutexattr.c clib/pthread.h
$(libunixobj)/pthread/newnode.o: pthread/newnode.c clib/pthread.h
$(libunixobj)/pthread/once.o: pthread/once.c clib/pthread.h
$(libunixobj)/pthread/rwlock.o: pthread/rwlock.c clib/pthread.h
$(libunixobj)/pthread/rwlockattr.o: pthread/rwlockattr.c clib/pthread.h
$(libunixobj)/pthread/self.o: pthread/self.c clib/pthread.h
$(libunixobj)/pthread/signal.o: pthread/signal.c clib/pthread.h
$(libunixobj)/pthread/testcancel.o: pthread/testcancel.c clib/pthread.h

$(libunixobj)/pthread/_context.o: pthread/_context.s
$(libunixobj)/pthread/_exit.o: pthread/_exit.s
$(libunixobj)/pthread/_ints.o: pthread/_ints.s
$(libunixobj)/pthread/_yield.o: pthread/_yield.s

$(libunixobj)/pwd/fgetpwent.o: pwd/fgetpwent.c
$(libunixobj)/pwd/fgetpwentr.o: pwd/fgetpwentr.c
$(libunixobj)/pwd/getlogin.o: pwd/getlogin.c
$(libunixobj)/pwd/getpw.o: pwd/getpw.c
$(libunixobj)/pwd/getpwent.o: pwd/getpwent.c
$(libunixobj)/pwd/getpwnam.o: pwd/getpwnam.c
$(libunixobj)/pwd/getpwnam_r.o: pwd/getpwnam_r.c
$(libunixobj)/pwd/getpwuid.o: pwd/getpwuid.c
$(libunixobj)/pwd/getpwuid_r.o: pwd/getpwuid_r.c
$(libunixobj)/pwd/putpwent.o: pwd/putpwent.c
$(libunixobj)/pwd/pwdread.o: pwd/pwdread.c

$(libunixobj)/grp/fgetgrent.o: grp/fgetgrent.c
$(libunixobj)/grp/fgetgrentr.o: grp/fgetgrentr.c
$(libunixobj)/grp/getgroups.o: grp/getgroups.c
$(libunixobj)/grp/getgrent.o: grp/getgrent.c
$(libunixobj)/grp/getgrnam.o: grp/getgrnam.c
$(libunixobj)/grp/getgrnam_r.o: grp/getgrnam_r.c
$(libunixobj)/grp/getgrgid.o: grp/getgrgid.c
$(libunixobj)/grp/getgrgid_r.o: grp/getgrgid_r.c
$(libunixobj)/grp/grpread.o: grp/grpread.c

$(libunixobj)/resolv/base64.o: resolv/base64.c
$(libunixobj)/resolv/gethnamaddr.o: resolv/gethnamaddr.c
$(libunixobj)/resolv/herror.o: resolv/herror.c
$(libunixobj)/resolv/inet_addr.o: resolv/inet_addr.c
$(libunixobj)/resolv/inet_neta.o: resolv/inet_neta.c
$(libunixobj)/resolv/inet_net_ntop.o: resolv/inet_net_ntop.c
$(libunixobj)/resolv/inet_net_pton.o: resolv/inet_net_pton.c
$(libunixobj)/resolv/inet_ntop.o: resolv/inet_ntop.c
$(libunixobj)/resolv/inet_pton.o: resolv/inet_pton.c
$(libunixobj)/resolv/nsap_addr.o: resolv/nsap_addr.c
$(libunixobj)/resolv/ns_name.o: resolv/ns_name.c
$(libunixobj)/resolv/ns_netint.o: resolv/ns_netint.c
$(libunixobj)/resolv/ns_parse.o: resolv/ns_parse.c
$(libunixobj)/resolv/ns_print.o: resolv/ns_print.c
$(libunixobj)/resolv/ns_samedomain.o: resolv/ns_samedomain.c
$(libunixobj)/resolv/ns_ttl.o: resolv/ns_ttl.c
$(libunixobj)/resolv/res_comp.o: resolv/res_comp.c
$(libunixobj)/resolv/res_data.o: resolv/res_data.c
$(libunixobj)/resolv/res_debug.o: resolv/res_debug.c
$(libunixobj)/resolv/res_hconf.o: resolv/res_hconf.c
$(libunixobj)/resolv/res_init.o: resolv/res_init.c
$(libunixobj)/resolv/res_mkquery.o: resolv/res_mkquery.c
$(libunixobj)/resolv/res_query.o: resolv/res_query.c
$(libunixobj)/resolv/res_send.o: resolv/res_send.c

$(libunixobj)/resource/getegid.o: resource/getegid.c
$(libunixobj)/resource/geteuid.o: resource/geteuid.c
$(libunixobj)/resource/getgid.o: resource/getgid.c
$(libunixobj)/resource/getpgrp.o: resource/getpgrp.c
$(libunixobj)/resource/getpid.o: resource/getpid.c
$(libunixobj)/resource/getppid.o: resource/getppid.c
$(libunixobj)/resource/getprior.o: resource/getprior.c
$(libunixobj)/resource/getrlimit.o: resource/getrlimit.c
$(libunixobj)/resource/getrusage.o: resource/getrusage.c
$(libunixobj)/resource/getuid.o: resource/getuid.c
$(libunixobj)/resource/initialise.o: resource/initialise.c
$(libunixobj)/resource/seteuid.o: resource/seteuid.c
$(libunixobj)/resource/setgid.o: resource/setgid.c
$(libunixobj)/resource/setpgid.o: resource/setpgid.c
$(libunixobj)/resource/setpgrp.o: resource/setpgrp.c
$(libunixobj)/resource/setprior.o: resource/setprior.c
$(libunixobj)/resource/setrlimit.o: resource/setrlimit.c
$(libunixobj)/resource/setuid.o: resource/setuid.c
$(libunixobj)/resource/ulimit.o: resource/ulimit.c

$(libunixobj)/scl/_kcurskch.o: scl/_kcurskch.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kernel.o: scl/_kernel.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kgetenv.o: scl/_kgetenv.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kosargs.o: scl/_kosargs.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kosbget.o: scl/_kosbget.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kosbput.o: scl/_kosbput.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kosbyte.o: scl/_kosbyte.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_koscli.o: scl/_koscli.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kosfile.o: scl/_kosfile.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kosfind.o: scl/_kosfind.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kosgbpb.o: scl/_kosgbpb.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kosrdch.o: scl/_kosrdch.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kosword.o: scl/_kosword.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_koswrch.o: scl/_koswrch.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_ksetenv.o: scl/_ksetenv.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_kswi_c.o: scl/_kswi_c.s clib/unixlib/asm_dec.s
$(libunixobj)/scl/_memchk.o: scl/_memchk.s clib/unixlib/asm_dec.s

$(libunixobj)/signal/coredump.o: signal/coredump.c
$(libunixobj)/signal/init.o: signal/init.c
$(libunixobj)/signal/kill.o: signal/kill.c
$(libunixobj)/signal/pause.o: signal/pause.c
$(libunixobj)/signal/post.o: signal/post.c
$(libunixobj)/signal/raise.o: signal/raise.c
$(libunixobj)/signal/setup.o: signal/setup.c
$(libunixobj)/signal/sigaction.o: signal/sigaction.c
$(libunixobj)/signal/sigaddset.o: signal/sigaddset.c
$(libunixobj)/signal/sigaltstk.o: signal/sigaltstk.c
$(libunixobj)/signal/sigblock.o: signal/sigblock.c
$(libunixobj)/signal/sigdelset.o: signal/sigdelset.c
$(libunixobj)/signal/siginfo.o: signal/siginfo.c
$(libunixobj)/signal/sigismembe.o: signal/sigismembe.c
$(libunixobj)/signal/signal.o: signal/signal.c
$(libunixobj)/signal/signame.o: signal/signame.c
$(libunixobj)/signal/sigpause.o: signal/sigpause.c
$(libunixobj)/signal/sigpending.o: signal/sigpending.c
$(libunixobj)/signal/sigprocmsk.o: signal/sigprocmsk.c
$(libunixobj)/signal/sigsetmask.o: signal/sigsetmask.c
$(libunixobj)/signal/sigsets.o: signal/sigsets.c
$(libunixobj)/signal/sigstack.o: signal/sigstack.c
$(libunixobj)/signal/sigsuspend.o: signal/sigsuspend.c
$(libunixobj)/signal/sigwakeup.o: signal/sigwakeup.c
$(libunixobj)/signal/sleep.o: signal/sleep.c
$(libunixobj)/signal/sigexec.o: signal/sigexec.s clib/unixlib/asm_dec.s
$(libunixobj)/signal/_coredump.o: signal/_coredump.s clib/unixlib/asm_dec.s
$(libunixobj)/signal/_signal.o: signal/_signal.s clib/unixlib/asm_dec.s

$(libunixobj)/stat/mknod.o: stat/mknod.c
$(libunixobj)/stat/mkfifo.o: stat/mkfifo.c

$(libunixobj)/stdio/clearerr.o: stdio/clearerr.c
$(libunixobj)/stdio/fclose.o: stdio/fclose.c
$(libunixobj)/stdio/feof.o: stdio/feof.c
$(libunixobj)/stdio/ferror.o: stdio/ferror.c
$(libunixobj)/stdio/fflush.o: stdio/fflush.c
$(libunixobj)/stdio/fgets.o: stdio/fgets.c
$(libunixobj)/stdio/filbuf.o: stdio/filbuf.c
$(libunixobj)/stdio/fileno.o: stdio/fileno.c
$(libunixobj)/stdio/flsbuf.o: stdio/flsbuf.c
$(libunixobj)/stdio/fopen.o: stdio/fopen.c
$(libunixobj)/stdio/fpos.o: stdio/fpos.c
$(libunixobj)/stdio/fputs.o: stdio/fputs.c
$(libunixobj)/stdio/fread.o: stdio/fread.c
$(libunixobj)/stdio/freopen.o: stdio/freopen.c
$(libunixobj)/stdio/fwrite.o: stdio/fwrite.c
$(libunixobj)/stdio/getc.o: stdio/getc.c
$(libunixobj)/stdio/getdelim.o: stdio/getdelim.c
$(libunixobj)/stdio/gets.o: stdio/gets.c
$(libunixobj)/stdio/getw.o: stdio/getw.c
$(libunixobj)/stdio/newstream.o: stdio/newstream.c
$(libunixobj)/stdio/perror.o: stdio/perror.c
$(libunixobj)/stdio/popen.o: stdio/popen.c
$(libunixobj)/stdio/print.o: stdio/print.c
$(libunixobj)/stdio/printf.o: stdio/printf.c
$(libunixobj)/stdio/putc.o: stdio/putc.c
$(libunixobj)/stdio/puts.o: stdio/puts.c
$(libunixobj)/stdio/putw.o: stdio/putw.c
$(libunixobj)/stdio/remove.o: stdio/remove.c
$(libunixobj)/stdio/rename.o: stdio/rename.c
$(libunixobj)/stdio/scan.o: stdio/scan.c
$(libunixobj)/stdio/scanf.o: stdio/scanf.c
$(libunixobj)/stdio/setbuf.o: stdio/setbuf.c
$(libunixobj)/stdio/stdio.o: stdio/stdio.c
$(libunixobj)/stdio/tmpfile.o: stdio/tmpfile.c
$(libunixobj)/stdio/ungetc.o: stdio/ungetc.c
$(libunixobj)/stdio/_printf.o: stdio/_printf.s clib/unixlib/asm_dec.s

$(libunixobj)/stdlib/drand48.o: stdlib/drand48.c
$(libunixobj)/stdlib/erand48.o: stdlib/erand48.c
$(libunixobj)/stdlib/jrand48.o: stdlib/jrand48.c
$(libunixobj)/stdlib/lcong48.o: stdlib/lcong48.c
$(libunixobj)/stdlib/lrand48.o: stdlib/lrand48.c
$(libunixobj)/stdlib/mrand48.o: stdlib/mrand48.c
$(libunixobj)/stdlib/nrand48.o: stdlib/nrand48.c
$(libunixobj)/stdlib/_rand48.o: stdlib/_rand48.c
$(libunixobj)/stdlib/rand.o: stdlib/rand.c
$(libunixobj)/stdlib/random.o: stdlib/random.c
$(libunixobj)/stdlib/realpath.o: stdlib/realpath.c
$(libunixobj)/stdlib/seed48.o: stdlib/seed48.c
$(libunixobj)/stdlib/srand48.o: stdlib/srand48.c
$(libunixobj)/stdlib/strtof.o: stdlib/strtof.c
$(libunixobj)/stdlib/strtold.o: stdlib/strtold.c

$(libunixobj)/string/strchrnul.o: string/strchrnul.c
$(libunixobj)/string/strndup.o: string/strndup.c
$(libunixobj)/string/swab.o: string/swab.c

$(libunixobj)/sys/alarm.o: sys/alarm.c
$(libunixobj)/sys/brk.o: sys/brk.c
$(libunixobj)/sys/debug.o: sys/debug.c
$(libunixobj)/sys/errlist.o: sys/errlist.c
$(libunixobj)/sys/exec.o: sys/exec.c
$(libunixobj)/sys/execl.o: sys/execl.c
$(libunixobj)/sys/execle.o: sys/execle.c
$(libunixobj)/sys/execlp.o: sys/execlp.c
$(libunixobj)/sys/execv.o: sys/execv.c
$(libunixobj)/sys/execvp.o: sys/execvp.c
$(libunixobj)/sys/mman.o: sys/mman.c
$(libunixobj)/sys/os.o: sys/os.c
$(libunixobj)/sys/sigsetjmp.o: sys/sigsetjmp.c
$(libunixobj)/sys/syslib.o: sys/syslib.c
$(libunixobj)/sys/vfork.o: sys/vfork.c
$(libunixobj)/sys/_alloca.o: sys/_alloca.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_address.o: sys/_address.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_divsi3.o: sys/_divsi3.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_divtest.o: sys/_divtest.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_exec.o: sys/_exec.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_jmp.o: sys/_jmp.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_mem.o: sys/_mem.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_mman.o: sys/_mman.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_modsi3.o: sys/_modsi3.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_os.o: sys/_os.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_profile.o: sys/_profile.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_sigjmp.o: sys/_sigjmp.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_sdiv.o: sys/_sdiv.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_sdiv10.o: sys/_sdiv10.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_smod.o: sys/_smod.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_swi.o: sys/_swi.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_syslib.o: sys/_syslib.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_udiv.o: sys/_udiv.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_udiv10.o: sys/_udiv10.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_udivsi3.o: sys/_udivsi3.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_umod.o: sys/_umod.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_umodsi3.o: sys/_umodsi3.s clib/unixlib/asm_dec.s
$(libunixobj)/sys/_vfork.o: sys/_vfork.s clib/unixlib/asm_dec.s

$(libunixobj)/termios/cfgetispee.o: termios/cfgetispee.c
$(libunixobj)/termios/cfgetospee.o: termios/cfgetospee.c
$(libunixobj)/termios/cfsetispee.o: termios/cfsetispee.c
$(libunixobj)/termios/cfsetospee.o: termios/cfsetospee.c
$(libunixobj)/termios/ctermid.o: termios/ctermid.c
$(libunixobj)/termios/gtty.o: termios/gtty.c
$(libunixobj)/termios/stty.o: termios/stty.c
$(libunixobj)/termios/tcdrain.o: termios/tcdrain.c
$(libunixobj)/termios/tcflow.o: termios/tcflow.c
$(libunixobj)/termios/tcflush.o: termios/tcflush.c
$(libunixobj)/termios/tcgetattr.o: termios/tcgetattr.c
$(libunixobj)/termios/tcgetpgrp.o: termios/tcgetpgrp.c
$(libunixobj)/termios/tcsendbrk.o: termios/tcsendbrk.c
$(libunixobj)/termios/tcsetattr.o: termios/tcsetattr.c
$(libunixobj)/termios/tcsetpgrp.o: termios/tcsetpgrp.c

$(libunixobj)/time/adjtime.o: time/adjtime.c
$(libunixobj)/time/asctime.o: time/asctime.c
$(libunixobj)/time/broken.o: time/broken.c
$(libunixobj)/time/calendconv.o: time/calendconv.c
$(libunixobj)/time/ctime.o: time/ctime.c
$(libunixobj)/time/difftime.o: time/difftime.c
$(libunixobj)/time/ftime.o: time/ftime.c
$(libunixobj)/time/getitmr.o: time/getitmr.c
$(libunixobj)/time/gettod.o: time/gettod.c
$(libunixobj)/time/gmtime.o: time/gmtime.c
$(libunixobj)/time/gmtime_r.o: time/gmtime_r.c
$(libunixobj)/time/localtime.o: time/localtime.c
$(libunixobj)/time/localtime_r.o: time/localtime_r.c
$(libunixobj)/time/mktime.o: time/mktime.c
$(libunixobj)/time/setitmr.o: time/setitmr.c
$(libunixobj)/time/settod.o: time/settod.c
$(libunixobj)/time/stdtime.o: time/stdtime.c
$(libunixobj)/time/strftime.o: time/strftime.c
$(libunixobj)/time/time.o: time/time.c
$(libunixobj)/time/times.o: time/times.c
$(libunixobj)/time/tzset.o: time/tzset.c
$(libunixobj)/time/utimes.o: time/utimes.c
$(libunixobj)/time/_clock.o: time/_clock.s clib/unixlib/asm_dec.s

$(libunixobj)/unistd/chown.o: unistd/chown.c
$(libunixobj)/unistd/chroot.o: unistd/chroot.c
$(libunixobj)/unistd/getpass.o: unistd/getpass.c
$(libunixobj)/unistd/fchown.o: unistd/fchown.c
$(libunixobj)/unistd/link.o: unistd/link.c
$(libunixobj)/unistd/readlink.o: unistd/readlink.c
$(libunixobj)/unistd/symlink.o: unistd/symlink.c

$(libunixobj)/unix/access.o: unix/access.c
$(libunixobj)/unix/alphasort.o: unix/alphasort.c
$(libunixobj)/unix/chdir.o: unix/chdir.c
$(libunixobj)/unix/chmod.o: unix/chmod.c
$(libunixobj)/unix/close.o: unix/close.c
$(libunixobj)/unix/creat.o: unix/creat.c
$(libunixobj)/unix/dev.o: unix/dev.c
$(libunixobj)/unix/dirent.o: unix/dirent.c
$(libunixobj)/unix/dup.o: unix/dup.c
$(libunixobj)/unix/dup2.o: unix/dup2.c
$(libunixobj)/unix/fchmod.o: unix/fchmod.c
$(libunixobj)/unix/features.o: unix/features.c
$(libunixobj)/unix/fnmatch.o: unix/fnmatch.c
$(libunixobj)/unix/fcntl.o: unix/fcntl.c
$(libunixobj)/unix/fstat.o: unix/fstat.c
$(libunixobj)/unix/getdomain.o: unix/getdomain.c
$(libunixobj)/unix/getenv.o: unix/getenv.c
$(libunixobj)/unix/getdtables.o: unix/getdtables.c
$(libunixobj)/unix/gethostnam.o: unix/gethostnam.c
$(libunixobj)/unix/getcwd.o: unix/getcwd.c
$(libunixobj)/unix/getopt.o: unix/getopt.c
$(libunixobj)/unix/getsubopt.o: unix/getsubopt.c
$(libunixobj)/unix/glob.c: unix/glob.c
$(libunixobj)/unix/ioctl.o: unix/ioctl.c
$(libunixobj)/unix/isatty.o: unix/isatty.c
$(libunixobj)/unix/ispipe.o: unix/ispipe.c
$(libunixobj)/unix/lseek.o: unix/lseek.c
$(libunixobj)/unix/lstat.o: unix/lstat.c
$(libunixobj)/unix/mkdir.o: unix/mkdir.c
$(libunixobj)/unix/nice.o: unix/nice.c
$(libunixobj)/unix/open.o: unix/open.c
$(libunixobj)/unix/pathconf.o: unix/pathconf.c
$(libunixobj)/unix/pipe.o: unix/pipe.c
$(libunixobj)/unix/poll.o: unix/poll.c
$(libunixobj)/unix/proc.o: unix/proc.c
$(libunixobj)/unix/pselect.o: unix/poll.c
$(libunixobj)/unix/putenv.o: unix/putenv.c
$(libunixobj)/unix/read.o: unix/read.c
$(libunixobj)/unix/readv.o: unix/readv.c
$(libunixobj)/unix/reopen.o: unix/reopen.c
$(libunixobj)/unix/rmdir.o: unix/rmdir.c
$(libunixobj)/unix/select.o: unix/select.c
$(libunixobj)/unix/setenv.o: unix/setenv.c
$(libunixobj)/unix/stat.o: unix/stat.c
$(libunixobj)/unix/sync.o: unix/sync.c
$(libunixobj)/unix/sysconf.o: unix/sysconf.c
$(libunixobj)/unix/syslog.o: unix/syslog.c
$(libunixobj)/unix/truncate.o: unix/truncate.c
$(libunixobj)/unix/tty.o: unix/tty.c
$(libunixobj)/unix/umask.o: unix/umask.c
$(libunixobj)/unix/uname.o: unix/uname.c
$(libunixobj)/unix/unix.o: unix/unix.c
$(libunixobj)/unix/unlink.o: unix/unlink.c
$(libunixobj)/unix/utime.o: unix/utime.c
$(libunixobj)/unix/wait.o: unix/wait.c
$(libunixobj)/unix/wait3.o: unix/wait3.c
$(libunixobj)/unix/wait4.o: unix/wait4.c
$(libunixobj)/unix/waitpid.o: unix/waitpid.c
$(libunixobj)/unix/write.o: unix/write.c
$(libunixobj)/unix/writev.o: unix/writev.c
$(libunixobj)/unix/getpagesiz.o: unix/getpagesiz.s clib/unixlib/asm_dec.s

$(libunixobj)/wchar/btowc.o: wchar/btowc.c
$(libunixobj)/wchar/mblen.o: wchar/mblen.c
$(libunixobj)/wchar/mbrlen.o: wchar/mbrlen.c
$(libunixobj)/wchar/mbrtowc.o: wchar/mbrtowc.c
$(libunixobj)/wchar/mbsinit.o: wchar/mbsinit.c
$(libunixobj)/wchar/mbsnrtowcs.o: wchar/mbsnrtowcs.c
$(libunixobj)/wchar/mbsrtowcs.o: wchar/mbsrtowcs.c
$(libunixobj)/wchar/mbstowcs.o: wchar/mbstowcs.c
$(libunixobj)/wchar/mbtowc.o: wchar/mbtowc.c
$(libunixobj)/wchar/wcpcpy.o: wchar/wcpcpy.c
$(libunixobj)/wchar/wcpncpy.o: wchar/wcpncpy.c
$(libunixobj)/wchar/wcrtomb.o: wchar/wcrtomb.c
$(libunixobj)/wchar/wcscat.o: wchar/wcscat.c
$(libunixobj)/wchar/wcschr.o: wchar/wcschr.c
$(libunixobj)/wchar/wcscmp.o: wchar/wcscmp.c
$(libunixobj)/wchar/wcscoll.o: wchar/wcscoll.c
$(libunixobj)/wchar/wcscpy.o: wchar/wcscpy.c
$(libunixobj)/wchar/wcscspn.o: wchar/wcscspn.c
$(libunixobj)/wchar/wcsdup.o: wchar/wcsdup.c
$(libunixobj)/wchar/wcslen.o: wchar/wcslen.c
$(libunixobj)/wchar/wcsncat.o: wchar/wcsncat.c
$(libunixobj)/wchar/wcsncmp.o: wchar/wcsncmp.c
$(libunixobj)/wchar/wcsncpy.o: wchar/wcsncpy.c
$(libunixobj)/wchar/wcsnrtombs.o: wchar/wcsnrtombs.c
$(libunixobj)/wchar/wcspbrk.o: wchar/wcspbrk.c
$(libunixobj)/wchar/wcsrchr.o: wchar/wcsrchr.c
$(libunixobj)/wchar/wcsrtombs.o: wchar/wcsrtombs.c
$(libunixobj)/wchar/wcsspn.o: wchar/wcsspn.c
$(libunixobj)/wchar/wcsstr.o: wchar/wcsstr.c
$(libunixobj)/wchar/wcstod.o: wchar/wcstod.c
$(libunixobj)/wchar/wcstof.o: wchar/wcstof.c
$(libunixobj)/wchar/wcstok.o: wchar/wcstok.c
$(libunixobj)/wchar/wcstol.o: wchar/wcstol.c
$(libunixobj)/wchar/wcstold.o: wchar/wcstold.c
$(libunixobj)/wchar/wcstombs.o: wchar/wcstombs.c
$(libunixobj)/wchar/wcstoq.o: wchar/wcstoq.c
$(libunixobj)/wchar/wcstoul.o: wchar/wcstoul.c
$(libunixobj)/wchar/wcstouq.o: wchar/wcstouq.c
$(libunixobj)/wchar/wcswidth.o: wchar/wcswidth.c
$(libunixobj)/wchar/wcsxfrm.o: wchar/wcsxfrm.c
$(libunixobj)/wchar/wctob.o: wchar/wctob.c
$(libunixobj)/wchar/wctomb.o: wchar/wctomb.c
$(libunixobj)/wchar/wcwidth.o: wchar/wcwidth.c
$(libunixobj)/wchar/wmemchr.o: wchar/wmemchr.c
$(libunixobj)/wchar/wmemcmp.o: wchar/wmemcmp.c
$(libunixobj)/wchar/wmemcpy.o: wchar/wmemcpy.c
$(libunixobj)/wchar/wmemmove.o: wchar/wmemmove.c
$(libunixobj)/wchar/wmemset.o: wchar/wmemset.c
