dnl Process this file with autoconf to produce a configure script

AC_PREREQ(2.59)
AC_INIT([UnixLib], 4.00, [gcc@gccsdk.riscos.info], [libunixlib])
AC_CONFIG_SRCDIR([assert/assert.c])
AC_CONFIG_HEADER(config.h)

#AM_ENABLE_MULTILIB(, ..)

AC_CANONICAL_SYSTEM
AC_ARG_PROGRAM

# Set up automake.  Must come after AC_CANONICAL_SYSTEM
# Don't follow normal rules for GNU packages
AM_INIT_AUTOMAKE([foreign -Wall -Wno-portability])

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
AC_CHECK_TOOL(AS, as)
AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(OBJCOPY, objcopy)
AC_CHECK_TOOL(RANLIB, ranlib, :)
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

# For future libtool support
#AC_LIBTOOL_DLOPEN
#AM_PROG_LIBTOOL
#AC_SUBST(enable_shared)
#AC_SUBST(enable_static)

AC_CACHE_SAVE

#if test ${multilib} = yes; then
#  multilib_arg="--enable-multilib"
#else
#  multilib_arg=
#fi

#AC_CONFIG_FILES

AC_OUTPUT(Makefile test/Makefile \
  test/math/Makefile \
  test/stdio/Makefile \
  test/stdlib/Makefile \
  test/string/Makefile \
  test/strings/Makefile)
