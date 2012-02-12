dnl RISC OS dot net shared library

sinclude(./config/m4/libtool.m4)

AC_PREREQ(2.59)
AC_INIT([RISC OS dot net Library], [1.0.0], [leenoar@sky.com], [libriscosdotnet])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([config/m4])
AM_INIT_AUTOMAKE([1.9 foreign])

LT_INIT

AC_PROG_CC
AM_PROG_AS
AC_PROG_RANLIB

AC_PROG_LIBTOOL
AC_SUBST(enable_shared)
AC_SUBST(enable_static)

AC_OUTPUT([Makefile src/Makefile])
