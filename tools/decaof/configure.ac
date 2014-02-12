AC_PREREQ([2.64])
AC_INIT([decaof],[2.02],[http://gccsdk.riscos.info/])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AM_PROG_CC_C_O

AM_INIT_AUTOMAKE([-Wall foreign])
dnl Support silent build rules.
dnl Disable by either passing --disable-silent-rules to configure
dnl or passing V=1 to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_HEADER_STDC
dnl AC_CHECK_FUNCS(strdup strndup strncasecmp)
dnl AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h])
dnl AC_C_BIGENDIAN
AC_C_CONST

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
