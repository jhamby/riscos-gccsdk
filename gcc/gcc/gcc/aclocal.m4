nl See whether we can include both string.h and strings.h.
AC_DEFUN(GCC_HEADER_STRING,
[AC_CACHE_CHECK([whether string.h and strings.h may both be included],
  gcc_cv_header_string,
[AC_TRY_COMPILE([#include <string.h>
#include <strings.h>], , gcc_cv_header_string=yes, gcc_cv_header_string=no)])
if test $gcc_cv_header_string = yes; then
  AC_DEFINE(STRING_WITH_STRINGS)
fi
])

dnl See whether we need a declaration for a function.
dnl GCC_NEED_DECLARATION(FUNCTION [, EXTRA-HEADER-FILES])
AC_DEFUN(GCC_NEED_DECLARATION,
[AC_MSG_CHECKING([whether $1 must be declared])
AC_CACHE_VAL(gcc_cv_decl_needed_$1,
[AC_TRY_COMPILE([
#include <stdio.h>
#ifdef STRING_WITH_STRINGS
# include <string.h>
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# else
#  ifdef HAVE_STRINGS_H
#   include <strings.h>
#  endif
# endif
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifndef HAVE_RINDEX
#define rindex strrchr
#endif
#ifndef HAVE_INDEX
#define index strchr
#endif
$2],
[char *(*pfn) = (char *(*)) $1],
eval "gcc_cv_decl_needed_$1=no", eval "gcc_cv_decl_needed_$1=yes")])
if eval "test \"`echo '$gcc_cv_decl_needed_'$1`\" = yes"; then
  AC_MSG_RESULT(yes)
  gcc_tr_decl=NEED_DECLARATION_`echo $1 | tr 'abcdefghijklmnopqrstuvwxyz' 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'`
  AC_DEFINE_UNQUOTED($gcc_tr_decl)
else
  AC_MSG_RESULT(no)
fi
])dnl


