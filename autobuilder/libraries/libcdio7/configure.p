--- configure.orig	2008-01-18 12:07:08.000000000 -0800
+++ configure	2008-01-18 12:53:25.000000000 -0800
@@ -7249,11 +7249,11 @@
 { echo "$as_me:$LINENO: checking bitfield ordering in structs" >&5
 echo $ECHO_N "checking bitfield ordering in structs... $ECHO_C" >&6; }
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&5
-echo "$as_me: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&2;}
-   { (exit 1); exit 1; }; }
+   bf_lsbf=1; # { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&5
+#echo "$as_me: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 /* confdefs.h.  */
@@ -22741,11 +22741,11 @@
 { echo "$as_me:$LINENO: checking extern long timezone variable" >&5
 echo $ECHO_N "checking extern long timezone variable... $ECHO_C" >&6; }
 if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&5
-echo "$as_me: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&2;}
-   { (exit 1); exit 1; }; }
+  : #{ { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&5
+#echo "$as_me: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 /* confdefs.h.  */
@@ -23045,11 +23045,11 @@
   { echo "$as_me:$LINENO: checking whether time.h defines daylight and timezone variables" >&5
 echo $ECHO_N "checking whether time.h defines daylight and timezone variables... $ECHO_C" >&6; }
   if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&5
-echo "$as_me: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&2;}
-   { (exit 1); exit 1; }; }
+  : #{ { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&5
+#echo "$as_me: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&2;}
+#   { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 /* confdefs.h.  */
@@ -23115,11 +23115,11 @@
   { echo "$as_me:$LINENO: checking whether time.h defines tzname variable" >&5
 echo $ECHO_N "checking whether time.h defines tzname variable... $ECHO_C" >&6; }
   if test "$cross_compiling" = yes; then
-  { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&5
-echo "$as_me: error: cannot run test program while cross compiling
-See \`config.log' for more details." >&2;}
-   { (exit 1); exit 1; }; }
+  : #{ { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&5
+#echo "$as_me: error: cannot run test program while cross compiling
+#See \`config.log' for more details." >&2;}
+#  { (exit 1); exit 1; }; }
 else
   cat >conftest.$ac_ext <<_ACEOF
 /* confdefs.h.  */
