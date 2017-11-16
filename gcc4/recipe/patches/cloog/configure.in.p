--- configure.in.orig	2010-08-27 20:13:48.000000000 +0100
+++ configure.in	2017-11-16 17:17:12.818773614 +0000
@@ -41,7 +41,7 @@
 AC_PREREQ(2.13)
 AC_INIT(source/cloog.c)
 AC_CONFIG_AUX_DIR(autoconf)
-
+AC_CONFIG_MACRO_DIR([m4])
 dnl Every other copy of the package version number gets its value from here.
 AM_INIT_AUTOMAKE(cloog, 0.15)
 AC_CONFIG_HEADER(include/cloog/cloog-config.h)
@@ -338,12 +338,12 @@
 
 	AC_MSG_CHECKING([for version 0.10 (or later revision) of PPL])
 	AC_TRY_COMPILE([#include "ppl_c.h"],[
-	#if PPL_VERSION_MAJOR != 0 || PPL_VERSION_MINOR < 10
+	#if PPL_VERSION_MAJOR == 0 && PPL_VERSION_MINOR < 10
 	choke me
 	#endif
 	], [AC_MSG_RESULT([yes])], [AC_MSG_ERROR(Can't find correct version of PPL.) ])
 
-	LIBS="$LIBS -lppl_c -lppl -lgmpxx"
+	LIBS="$LIBS -lppl_c -lppl -lgmpxx -lm"
 	AC_DEFINE([CLOOG_PPL_BACKEND], 1, [Use the PPL backend])
 
 else
