#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([elftoolchain], [1.0], [http://sourceforge.net/projects/elftoolchain/])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AC_CONFIG_SRCDIR([.])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall foreign])
dnl Support silent build rules.
dnl Disable by either passing --disable-silent-rules to configure
dnl or passing V=1 to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LN_S
AM_PROG_LEX
AC_PROG_YACC
# AM_PROG_AR is not present in automake 1.11.1
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

LT_INIT

REQ_LIBARCHIVE_VERSION=2.8.4

AC_ARG_ENABLE(building-tools,
AS_HELP_STRING([--disable-building-tools],
  [do not build the ELF toolchain tools, only build ELF/DWARF libraries]),
BUILDING_TOOLS=$enableval,
BUILDING_TOOLS=yes)
AM_CONDITIONAL([BUILDING_TOOLS], [test x"${BUILDING_TOOLS}" = "xyes"])

# Check for libarchive (only when ELF toolchain tools are built).
AS_IF([test x"${BUILDING_TOOLS}" = "xyes"],
      [PKG_CHECK_MODULES(LIBARCHIVE, [libarchive >= $REQ_LIBARCHIVE_VERSION])
       AC_SUBST(LIBARCHIVE_CFLAGS)
       AC_SUBST(LIBARCHIVE_LIBS)])

# Determine LIBELF_ARCH, LIBELF_BYTEORDER and LIBELF_CLASS for native cross
# ($build == $host != $target) and canadian cross ($build != $host != $target)
# compilation using config.target.
AS_IF([test x"$host" != x"$target"],
      [AC_MSG_NOTICE([Native or Canadian cross compilation detected])
       . ${srcdir}/config.target
       AC_DEFINE_UNQUOTED([LIBELF_ARCH],[$libelf_arch],[libelf architecture to be supported.])
       AC_DEFINE_UNQUOTED([LIBELF_BYTEORDER],[$libelf_byteorder],[libelf byte order (ELFDATA2LSB or ELFDATA2MSB) to be supported.])
       AC_DEFINE_UNQUOTED([LIBELF_CLASS],[$libelf_class],[libelf class (ELFCLASS32 or ELFCLASS64) to be supported.])
      ])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h limits.h stddef.h stdint.h string.h strings.h sys/param.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_UID_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_FSEEKO
AC_FUNC_MMAP
AC_CHECK_FUNCS([ftruncate memmove memset munmap strcasecmp strchr strdup strerror strncasecmp strrchr strstr strtol strtoul strtoull strtoumax strmode])

AC_CACHE_CHECK([for utime],
               [my_cv_func_utime],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <utime.h>]],
                                                [[utime(0, 0)]])],
                               [my_cv_func_utime=yes],
                               [my_cv_func_utime=no])])
AS_IF([test x"$my_cv_func_utime" = "xyes"],
      [AC_DEFINE([HAVE_UTIME],[1],[Define to 1 if you have the `utime' function.])])

AC_CACHE_CHECK([for utimes],
               [my_cv_func_utimes],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sys/time.h>]],
                                                [[utimes(0, 0)]])],
                               [my_cv_func_utimes=yes],
                               [my_cv_func_utimes=no])])
AS_IF([test x"$my_cv_func_utimes" = "xyes"],
      [AC_DEFINE([HAVE_UTIMES],[1],[Define to 1 if you have the `utimes' function.])
       AC_CHECK_MEMBERS([struct stat.st_atimespec, struct stat.st_mtimespec, struct stat.st_atim, struct stat.st_mtim])
      ])

AC_CACHE_CHECK([for htobe32],
               [my_cv_func_htobe32],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <endian.h>]],
                                                [[htobe32(0)]])],
                               [my_cv_func_htobe32=yes],
                               [my_cv_func_htobe32=no])])
AS_IF([test x"$my_cv_func_htobe32" = "xyes"],
      [AC_DEFINE([HAVE_HTOBE32],[1],[Define to 1 if you have the `htobe32' function.])])

AC_CACHE_CHECK([for le32dec,be32dec,le64dec,be64dec],
               [my_cv_func_byteorder_extensions],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sys/endian>]],
                                                [[le32dec(0);be32dec(0);le64dec(0);be64dec(0)]])],
                               [my_cv_func_byteorder_extensions=yes],
                               [my_cv_func_byteorder_extensions=no])])
AS_IF([test x"$my_cv_func_byteorder_extensions" = "xyes"],
      [AC_DEFINE([HAVE_BYTEORDER_EXTENSIONS],[1],
                 [Define to 1 if you have the `le32dec', `be32dec', `le64dec' and `be64dec' functions in sys/endian.h.])])

AC_CACHE_CHECK([for roundup2],
               [my_cv_func_roundup2],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sys/param.h>]],
                                                [[roundup2(2,2)]])],
                               [my_cv_func_roundup2=yes],
                               [my_cv_func_roundup2=no])])
AS_IF([test x"$my_cv_func_roundup2" = "xyes"],
      [AC_DEFINE([HAVE_ROUNDUP2],[1],
                 [Define to 1 if you have the `roundup2' function.])])

AC_CACHE_CHECK([whether __RCSID works],
               [my_cv_check_rcsid],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[__RCSID("foo");]],[])],
                               [my_cv_check_rcsid=yes],
                               [my_cv_check_rcsid=no])])
AC_CACHE_CHECK([whether __FBSDID works],
               [my_cv_check_fbsdid],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[__FBSDID("foo");]],[])],
                               [my_cv_check_fbsdid=yes],
                               [my_cv_check_fbsdid=no])])
AC_CACHE_CHECK([whether __asm__ .ident works],
               [my_cv_check_asmid],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[__asm__(".ident\t\"foo\"");]],[])],
                               [my_cv_check_asmid=yes],
                               [my_cv_check_asmid=no])])
AS_IF([test x"$my_cv_check_rcsid" = "xyes"],
      [AC_DEFINE([HAVE_RCSID],[1],[Define to 1 if `__RCSID' is available.])],
      [test x"$my_cv_check_fbsdid" = "xyes"],
      [AC_DEFINE([HAVE_FBSDID],[1],[Define to 1 if `__FBSDID' is available.])],
      [test x"$my_cv_check_asmid" = "xyes"],
      [AC_DEFINE([HAVE_ASMIDENT],[1],[Define to 1 if `__asm__ .ident' is supported.])])

AC_CACHE_CHECK([for getprogname],
               [my_cv_func_getprogname],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdlib.h>]],
                                                [[getprogname()]])],
                               [my_cv_func_getprogname=yes],
                               [my_cv_func_getprogname=no])])
AC_CACHE_CHECK([for program_invocation_short_name],
               [my_cv_check_program_invocation_short_name],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#define _GNU_SOURCE
                                                  #include <errno.h>]],
                                                [[program_invocation_short_name]])],
                               [my_cv_check_program_invocation_short_name=yes],
                               [my_cv_check_program_invocation_short_name=no])])
AC_CACHE_CHECK([for __progname],
               [my_cv_check___progname],
               [AC_LINK_IFELSE([AC_LANG_PROGRAM([[extern const char *__progname;]],
                                                [[__progname]])],
                               [my_cv_check___progname=yes],
                               [my_cv_check___progname=no])])
AS_IF([test x"$my_cv_func_getprogname" = "xyes"],
      [AC_DEFINE([HAVE_GETPROGNAME],[1],[Define to 1 if you have the `getprogname' function.])],
      [test x"$my_cv_check_program_invocation_short_name" = "xyes"],
      [AC_DEFINE([HAVE_PROGRAM_INVOCATION_SHORT_NAME],[1],[Define to 1 if `program_invocation_short_name' is available.])],
      [test x"$my_cv_check___progname" = "xyes"],
      [AC_DEFINE([HAVE___PROGNAME],[1],[Define to 1 if `__progname' is available.])],
      [AC_MSG_ERROR([no getprogname() equivalent found])])

AC_CACHE_CHECK([for _BYTE_ORDER, _LITTLE_ENDIAN, _BIG_ENDIAN],
               [my_cv_check__byte_order],
               [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/endian.h>]],
                                                   [[int a = _BYTE_ORDER;
                                                     int b = _LITTLE_ENDIAN;
                                                     int c = _BIG_ENDIAN]])],
                               [my_cv_check__byte_order=yes],
                               [my_cv_check__byte_order=no])])
AS_IF([test x"$my_cv_check__byte_order" = "xyes"],
      [AC_DEFINE([HAVE__BYTE_ORDER],[1],[Define to 1 if _BYTE_ORDER is available.])])
AC_CACHE_CHECK([for __BYTE_ORDER, __LITTLE_ENDIAN, __BIG_ENDIAN],
               [my_cv_check___byte_order],
               [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <endian.h>]],
                                                   [[int a = __BYTE_ORDER;
                                                     int b = __LITTLE_ENDIAN;
                                                     int c = __BIG_ENDIAN]])],
                               [my_cv_check___byte_order=yes],
                               [my_cv_check___byte_order=no])])
AS_IF([test x"$my_cv_check___byte_order" = "xyes"],
      [AC_DEFINE([HAVE___BYTE_ORDER],[1],[Define to 1 if __BYTE_ORDER is available.])])
AS_IF([test x"$my_cv_check__byte_order" != "xyes" -a x"$my_cv_check___byte_order" != "xyes"],
      [AC_MSG_ERROR([no _BYTE_ORDER, _LITTLE_ENDIAN, _BIG_ENDIAN equivalent found])])

# pkg.m4 defines PKG_INSTALLDIR since 0.27.  Provide a fallback for older
# pkgconfig versions.
m4_ifdef([PKG_INSTALLDIR], [PKG_INSTALLDIR],
         [AC_ARG_WITH([pkgconfigdir],
                      [AS_HELP_STRING([--with-pkgconfigdir],
                      [install directory for pkg-config files])],
                       [],[with_pkgconfigdir='$(libdir)/pkgconfig'])
          AC_SUBST([pkgconfigdir], [${with_pkgconfigdir}])])

AC_CONFIG_FILES([GNUmakefile
                 addr2line/GNUmakefile
                 ar/GNUmakefile
                 as/GNUmakefile
                 brandelf/GNUmakefile
                 common/GNUmakefile
                 cxxfilt/GNUmakefile
                 elfcopy/GNUmakefile
                 elfdump/GNUmakefile
                 findtextrel/GNUmakefile
                 isa/GNUmakefile
                 ld/GNUmakefile
                 libdwarf/GNUmakefile
                 libelf/GNUmakefile
                 libelftc/GNUmakefile
                 nm/GNUmakefile
                 readelf/GNUmakefile
                 size/GNUmakefile
                 strings/GNUmakefile
                 libelf-elftoolchain.pc
                 libelftc-elftoolchain.pc
                 libdwarf-elftoolchain.pc])

AC_OUTPUT
