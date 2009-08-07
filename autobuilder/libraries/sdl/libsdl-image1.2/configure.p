--- configure.orig	2007-11-09 14:29:36.382726100 +0000
+++ configure	2007-11-09 14:30:57.267726100 +0000
@@ -20734,7 +20734,7 @@
   echo $ECHO_N "(cached) $ECHO_C" >&6
 else
   ac_check_lib_save_LIBS=$LIBS
-LIBS="-lpng  $LIBS"
+LIBS="-lpng -lz $LIBS"
 cat >conftest.$ac_ext <<_ACEOF
 /* confdefs.h.  */
 _ACEOF
@@ -20973,7 +20973,7 @@
   echo $ECHO_N "(cached) $ECHO_C" >&6
 else
   ac_check_lib_save_LIBS=$LIBS
-LIBS="-ltiff  $LIBS"
+LIBS="-ltiff -lz  $LIBS"
 cat >conftest.$ac_ext <<_ACEOF
 /* confdefs.h.  */
 _ACEOF
