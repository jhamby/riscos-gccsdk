AC_PREREQ([2.64])
AC_INIT([SysLogD],[0.05],[http://gccsdk.riscos.info/])

AC_PROG_CC
AM_PROG_AS

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
dnl Support silent build rules.
dnl Disable by either passing --disable-silent-rules to configure
dnl or passing V=1 to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# We need cmunge running on build machine:
AC_PATH_PROG([CMUNGE], [cmunge])
AS_IF([test "x${CMUNGE}" == "x"],
    [AC_MSG_ERROR([C Module Header Generator "cmunge" not found.])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
