--- configure.in.orig	2009-07-31 12:36:31.000000000 -0700
+++ configure.in	2009-07-31 12:36:41.000000000 -0700
@@ -205,7 +205,7 @@
 
 AM_CONDITIONAL(ENABLE_DEFAULTS_SERVICE, [test "x$enable_defaults_service" != "xno"])
 
-ORBIT_IDL="`$PKG_CONFIG --variable=orbit_idl ORBit-2.0`"
+#ORBIT_IDL="`$PKG_CONFIG --variable=orbit_idl ORBit-2.0`"
 AC_SUBST(ORBIT_IDL)
 
 AC_CHECK_HEADER(pthread.h, have_pthreads=yes)
