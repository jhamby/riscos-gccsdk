--- configure.old	2003-04-23 13:49:03.000000000 +0100
+++ configure	2003-04-23 12:49:24.000000000 +0100
@@ -5423,11 +5423,11 @@
   echo "$as_me:$LINENO: checking for broken regexec" >&5
 echo $ECHO_N "checking for broken regexec... $ECHO_C" >&6
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
