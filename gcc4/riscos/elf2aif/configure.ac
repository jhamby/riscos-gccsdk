AC_PREREQ([2.64])
AC_INIT([elf2aif],[0.06],[http://gccsdk.riscos.info/])

AC_CONFIG_HEADERS([config.h])

AC_PROG_CC

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
dnl Support silent build rules.
dnl Disable by either passing --disable-silent-rules to configure
dnl or passing V=1 to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h])
AC_C_CONST

AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
