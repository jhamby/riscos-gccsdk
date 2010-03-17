dnl Stubs for the RISC OS SharedCLibrary Runtime Library
dnl Derived from libgfortran/libunixlib and designed to fit into the GCC build
dnl structure and be automatically built as it is.
dnl Process this file with autoconf to produce a configure script
dnl Written by John Tytgat <John.Tytgat@aaug.net> based on the work done for
dnl UnixLib.

# Need this to obtain the GCC_NO_EXECUTABLES line.
sinclude(../config/no-executables.m4)

AC_PREREQ(2.59)
AC_INIT([Stubs for the RISC OS SharedCLibrary Runtime Library], 2.0, [http://gccsdk.riscos.info/], [libscl])
AC_CONFIG_SRCDIR([crt/crt0-scl.s])
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
    libscl_topdir="${srcdir}/../.."
    with_target_subdir=
  elif test -z "${with_target_subdir}"; then
    libscl_topdir="${srcdir}/.."
  else
    if test "${with_target_subdir}" != "."; then
      libscl_topdir="${srcdir}/${with_multisrctop}../.."
    else
      libscl_topdir="${srcdir}/${with_multisrctop}.."
    fi
  fi
else
  libscl_topdir="${srcdir}/.."
fi
AC_SUBST(libscl_topdir)

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

AC_CACHE_SAVE

if test ${multilib} = yes; then
  multilib_arg="--enable-multilib"
else
  multilib_arg=
fi

#AC_CONFIG_FILES

AC_OUTPUT(Makefile)
