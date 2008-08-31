AC_PREREQ(2.59)
AC_INIT([mkresfs],[1.04],[http://gccsdk.riscos.info/])

AC_CONFIG_HEADERS([config.h])

AC_PROG_CC

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h stdio.h stdbool.h string.h ctype.h])
AC_C_CONST

AC_CHECK_FUNCS([memset])
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
