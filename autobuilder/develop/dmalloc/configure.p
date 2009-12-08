--- configure.orig	2009-12-07 17:39:15.000000000 -0800
+++ configure	2009-12-07 17:40:56.000000000 -0800
@@ -5306,7 +5306,7 @@
 
 echo "$as_me:$LINENO: checking basic-block size" >&5
 echo $ECHO_N "checking basic-block size... $ECHO_C" >&6
-ac_cv_page_size=0
+ac_cv_page_size=12
 if test $ac_cv_page_size = 0; then
    if test "$cross_compiling" = yes; then
   { { echo "$as_me:$LINENO: error: cannot run test program while cross compiling
