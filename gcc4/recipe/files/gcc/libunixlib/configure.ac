dnl UnixLib RISC OS C Runtime Library
dnl Derived from libgfortran and designed to fit into the GCC build
dnl structure and be automatically built as it is.
dnl Process this file with autoconf to produce a configure script
dnl Written by Nick Burrett <nick@sqrt.co.uk>

# Need this to obtain the GCC_NO_EXECUTABLES line.
sinclude(../config/no-executables.m4)
# Need this to make sure we're using overruled AC_LIBTOOL_SYS_DYNAMIC_LINKER
sinclude(./libtool.m4)

AC_PREREQ(2.59)
AC_INIT([RISC OS UnixLib C Runtime Library], 5.0, [http://gccsdk.riscos.info/], [libunixlib])
AC_CONFIG_SRCDIR([assert/assert.c])
AC_CONFIG_HEADER(config.h)

AC_CONFIG_AUX_DIR(..)

# Options
AC_MSG_CHECKING([for --enable-version-specific-runtime-libs])
AC_ARG_ENABLE(version-specific-runtime-libs,
[  --enable-version-specific-runtime-libs    Specify that runtime libraries should be installed in a compiler-specific directory ],
[case "$enableval" in
 yes) version_specific_libs=yes ;;
 no)  version_specific_libs=no ;;
 *)   AC_MSG_ERROR([Unknown argument to enable/disable version-specific libs]);;
 esac],
[version_specific_libs=no])
AC_MSG_RESULT($version_specific_libs)

if test "${srcdir}" = "."; then
  if test -n "${with_build_subdir}"; then
    libunixlib_topdir="${srcdir}/../.."
    with_target_subdir=
  elif test -z "${with_target_subdir}"; then
    libunixlib_topdir="${srcdir}/.."
  else
    if test "${with_target_subdir}" != "."; then
      libunixlib_topdir="${srcdir}/${with_multisrctop}../.."
    else
      libunixlib_topdir="${srcdir}/${with_multisrctop}.."
    fi
  fi
else
  libunixlib_topdir="${srcdir}/.."
fi
AC_SUBST(libunixlib_topdir)

# Gets build, host, target, *_vendor, *_cpu, *_os, etc.
AC_CANONICAL_SYSTEM
target_alias=${target_alias-$host_alias}
AC_ARG_PROGRAM

# Set up automake.  Must come after AC_CANONICAL_SYSTEM
# Don't follow normal rules for GNU packages
AM_INIT_AUTOMAKE([no-define foreign -Wall -Wno-portability])
AM_MAINTAINER_MODE
AM_ENABLE_MULTILIB(, ..)

# This line is to avoid the AC_PROG_CC check from testing whether the
# compiler works because at this stage we are most likely to not have
# a run-time library present to link against.
GCC_NO_EXECUTABLES

# Calculate toolexeclibdir
# Also toolexecdir, though it's only used in toolexeclibdir
case ${version_specific_libs} in
  yes)
    # Need the gcc compiler version to know where to install libraries
    # and header files if --enable-version-specific-runtime-libs option
    # is selected.
    toolexecdir='$(libdir)/gcc/$(target_alias)'
    toolexeclibdir='$(toolexecdir)/'${gcc_version}'$(MULTISUBDIR)'
    ;;
  no)
    if test -n "$with_cross_host" &&
       test x"$with_cross_host" != x"no"; then
      # Install a library built with a cross compiler in tooldir, not libdir.
      toolexecdir='$(exec_prefix)/$(target_alias)'
      toolexeclibdir='$(toolexecdir)/lib'
    else
      toolexecdir='$(libdir)/gcc-lib/$(target_alias)'
      toolexeclibdir='$(libdir)'
    fi
    multi_os_directory=`$CC -print-multi-os-directory`
    case $multi_os_directory in
      .) ;; # Avoid trailing /.
      *) toolexeclibdir=$toolexeclibdir/$multi_os_directory ;;
    esac
    ;;
esac
AC_SUBST(toolexecdir)
AC_SUBST(toolexeclibdir)

# Check the compiler.
# The same as in boehm-gc and libstdc++. Have to borrow it from there.
# We must force CC to /not/ be precious variables; otherwise
# the wrong, non-multilib-adjusted value will be used in multilibs.
# As a side effect, we have to subst CFLAGS ourselves.

m4_rename([_AC_ARG_VAR_PRECIOUS],[real_PRECIOUS])
m4_define([_AC_ARG_VAR_PRECIOUS],[])
AC_PROG_CC
AC_PROG_CPP
m4_rename([real_PRECIOUS],[_AC_ARG_VAR_PRECIOUS])
AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)

# Define assember for automake.
AM_PROG_AS
AC_PROG_LN_S
AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(OBJCOPY, objcopy)
AC_CHECK_TOOL(RANLIB, ranlib, ranlib-not-found-in-path-error)
AC_PROG_MAKE_SET
# Get 'install' or 'install-sh' and its variants
AC_PROG_INSTALL

dnl Conditional to be used to figure out if we're building SharedCLibrary
dnl or UnixLib:
AM_CONDITIONAL([UNIXLIB_BUILDING_SCL], [echo $CC | grep -- -mlibscl])
dnl Conditional to figure out if we want support for chunked stacks or not:
AM_CONDITIONAL([UNIXLIB_CHUNKED_STACK], [test x"${target}" = xarm-unknown-riscos])

# Calculate toolexeclibdir
# Also toolexecdir, though it's only used in toolexeclibdir
case ${version_specific_libs} in
  yes)
    # Need the gcc compiler version to know where to install libraries
    # and header files if --enable-version-specific-runtime-libs option
    # is selected.
    toolexecdir='$(libdir)/gcc/$(target_alias)'
    toolexeclibdir='$(toolexecdir)/'${gcc_version}'$(MULTISUBDIR)'
    ;;
  no)
    if test -n "$with_cross_host" &&
       test x"$with_cross_host" != x"no"; then
      # Install a library built with a cross compiler in tooldir, not libdir.
      toolexecdir='$(exec_prefix)/$(target_alias)'
      toolexeclibdir='$(toolexecdir)/lib'
    else
      toolexecdir='$(libdir)/gcc-lib/$(target_alias)'
      toolexeclibdir='$(libdir)'
    fi
    multi_os_directory=`$CC -print-multi-os-directory`
    case $multi_os_directory in
      .) ;; # Avoid trailing /.
      *) toolexeclibdir=$toolexeclibdir/$multi_os_directory ;;
    esac
    ;;
esac
AC_SUBST(toolexecdir)
AC_SUBST(toolexeclibdir)

# Don't build a shared library for SCL:
if [[ -n "`echo $CC | grep -- -mlibscl`" ]]; then
  enable_shared=no
fi

# For libtool support
AC_LIBTOOL_DLOPEN
AM_PROG_LIBTOOL
AC_SUBST(enable_shared)
AC_SUBST(enable_static)

AC_CACHE_SAVE

if test ${multilib} = yes; then
  multilib_arg="--enable-multilib"
else
  multilib_arg=
fi

#AC_CONFIG_FILES

# The test process-redirect needs its own Makefile and test Obey script.
AC_OUTPUT(Makefile test/Makefile test/assert/Makefile test/basic/Makefile test/ctype/Makefile test/dirent/Makefile test/env/Makefile test/errno/Makefile test/filename/Makefile test/grp/Makefile test/iconv/Makefile test/math/Makefile test/pthread/Makefile test/scl/Makefile test/setjmp/Makefile test/stdio/Makefile test/stdlib/Makefile test/string/Makefile test/strings/Makefile test/sys/Makefile test/unix/Makefile)
