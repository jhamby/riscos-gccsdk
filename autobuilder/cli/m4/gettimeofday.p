--- lib/sys_time.in.h.orig	2009-01-01 12:05:59.000000000 -0800
+++ lib/sys_time.in.h	2009-01-01 12:06:10.000000000 -0800
@@ -43,7 +43,7 @@
 };
 # endif
 
-# if @REPLACE_GETTIMEOFDAY@
+# if 0
 #  undef gettimeofday
 #  define gettimeofday rpl_gettimeofday
 int gettimeofday (struct timeval *restrict, void *restrict);
