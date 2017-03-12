--- base/time_.h.orig	2013-05-20 16:07:45.855912111 +0100
+++ base/time_.h	2013-05-20 16:08:06.639857249 +0100
@@ -45,7 +45,7 @@
 #  endif
 #else
 #  include <time.h>
-#  if !defined(__DECC) && !defined(__MWERKS__)
+#  if !defined(__DECC) && !defined(__MWERKS__) && !defined(__GNUC__)
 struct timeval {
     long tv_sec, tv_usec;
 };
