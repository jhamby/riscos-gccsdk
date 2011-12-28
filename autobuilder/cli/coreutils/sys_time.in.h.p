--- lib/sys_time.in.h.orig	2011-12-28 13:18:48.000000000 +0000
+++ lib/sys_time.in.h	2011-12-28 13:19:31.000000000 +0000
@@ -71,7 +71,7 @@
 
 # if @GNULIB_GETTIMEOFDAY@
 #  if @REPLACE_GETTIMEOFDAY@
-#   if !(defined __cplusplus && defined GNULIB_NAMESPACE)
+#   if !(defined __cplusplus && defined GNULIB_NAMESPACE) && !(defined __riscos__)
 #    undef gettimeofday
 #    define gettimeofday rpl_gettimeofday
 #   endif
