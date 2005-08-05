--- configure.orig	2005-08-05 16:01:28.000000000 +0100
+++ configure	2005-08-05 17:10:15.000000000 +0100
@@ -23167,11 +23167,11 @@
 echo "$as_me:$LINENO: checking bitfield ordering in structs" >&5
 echo $ECHO_N "checking bitfield ordering in structs... $ECHO_C" >&6
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&5
-echo "$as_me: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&2;}
-   { (exit 1); exit 1; }; }
+#  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&5
+#echo "$as_me: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&2;}
+: #   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 /* confdefs.h.  */
