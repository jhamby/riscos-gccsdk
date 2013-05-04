--- configure.in.orig	2013-05-01 16:15:33.235945936 +0200
+++ configure.in	2013-05-01 16:15:15.484169106 +0200
@@ -41,7 +41,7 @@ dnl ************************************
 AC_PREREQ(2.13)
 AC_INIT(source/cloog.c)
 AC_CONFIG_AUX_DIR(autoconf)
-
+AC_CONFIG_MACRO_DIR([m4])
 dnl Every other copy of the package version number gets its value from here.
 AM_INIT_AUTOMAKE(cloog, 0.15)
 AC_CONFIG_HEADER(include/cloog/cloog-config.h)
@@ -343,7 +343,7 @@ if test "x$with_ppl" != "x" -a "x$with_p
 	#endif
 	], [AC_MSG_RESULT([yes])], [AC_MSG_ERROR(Can't find correct version of PPL.) ])
 
-	LIBS="$LIBS -lppl_c -lppl -lgmpxx"
+	LIBS="$LIBS -lppl_c -lppl -lgmpxx -lm"
 	AC_DEFINE([CLOOG_PPL_BACKEND], 1, [Use the PPL backend])
 
 else
