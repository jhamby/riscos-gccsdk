AC_PREREQ([2.64])

AC_INIT([RISC OS Dynamic Loader], [1.9.9], [http://gccsdk.riscos.info/])
AM_INIT_AUTOMAKE([1.9 foreign])
dnl Support silent build rules.
dnl Disable by either passing --disable-silent-rules to configure
dnl or passing V=1 to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AM_PROG_AS
AC_PROG_RANLIB

AC_CHECK_TOOL(OBJCOPY, objcopy)
AC_CHECK_TOOL(LN, ln)
AC_CHECK_TOOL(AR, ar)

AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#ifndef __ARM_EABI__
# error macro not defined
#endif
]])], [arm_eabi=yes], [arm_eabi=no])
AM_CONDITIONAL([ARM_EABI], [test x$arm_eabi = xyes])
AC_SUBST(ARM_EABI)

AC_OUTPUT(Makefile d-link/Makefile d-link/libdl/Makefile)
