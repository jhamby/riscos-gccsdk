--- configure.old	2004-02-04 13:09:28.000000000 +0000
+++ configure	2004-02-04 13:10:00.000000000 +0000
@@ -8650,11 +8650,11 @@
 # it doesn't seem to exist, which is odd. We need to use
 # AC_TRY_RUN.
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&5
-echo "$as_me: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&2;}
-   { (exit 1); exit 1; }; }
+: #  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&5
+#echo "$as_me: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 #line $LINENO "configure"
