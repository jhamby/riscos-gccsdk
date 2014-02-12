AC_PREREQ([2.64])
AC_INIT([ln],[1.03],[http://gccsdk.riscos.info/])

# AC_CANONICAL_TARGET will make sure that when having host != target the ln
# binary gets installed as <target>-ln instead avoiding it being picked up
# when building the native RISC OS compiler (otherwise we have build
# problems).
AC_CANONICAL_TARGET

AC_PROG_CC

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
dnl Support silent build rules.
dnl Disable by either passing --disable-silent-rules to configure
dnl or passing V=1 to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
