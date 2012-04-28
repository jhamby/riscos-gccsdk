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

AC_PATH_PROG(MCS, mcs, no)

AC_PROG_LIBTOOL
AC_SUBST(enable_shared)
AC_SUBST(enable_static)
AC_SUBST(MCS)

###################
# DOXYGEN SUPPORT
###################

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN([$PACKAGE_NAME],[doxygen.cfg])

##################

AC_OUTPUT([Makefile src/Makefile assembly/Makefile test/Makefile])
