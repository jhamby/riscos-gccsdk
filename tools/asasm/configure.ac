AC_PREREQ([2.64])
AC_INIT([asasm],[2.01],[http://gccsdk.riscos.info/])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SUBDIRS([elftoolchain decaof])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall foreign])

AC_USE_SYSTEM_EXTENSIONS

AC_PROG_CC
AM_PROG_CC_C_O
AC_GNU_SOURCE

LT_INIT

AC_HEADER_STDC
AC_CHECK_FUNCS(strdup strndup strncasecmp strcasecmp)
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h])
AC_C_BIGENDIAN
AC_C_CONST

AC_CONFIG_FILES([Makefile src/Makefile unittests/compare/Makefile unittests/reloc/Makefile])
AC_OUTPUT
