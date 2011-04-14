--- configure-old.in	2010-08-27 12:13:48.000000000 -0700
+++ configure.in	2010-08-31 10:19:13.000000000 -0700
@@ -41,7 +41,7 @@
 AC_PREREQ(2.13)
 AC_INIT(source/cloog.c)
 AC_CONFIG_AUX_DIR(autoconf)
-
+AC_CONFIG_MACRO_DIR([m4])
 dnl Every other copy of the package version number gets its value from here.
 AM_INIT_AUTOMAKE(cloog, 0.15)
 AC_CONFIG_HEADER(include/cloog/cloog-config.h)

