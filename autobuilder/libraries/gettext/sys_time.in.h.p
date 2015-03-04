--- gettext-runtime/gnulib-lib/sys_time.in.h	2014-10-16 06:00:09.000000000 +0000
+++ gettext-runtime/gnulib-lib/sys_time.in.h	2015-03-04 21:37:16.250883000 +0000
@@ -17,6 +17,9 @@
 
 /* Written by Paul Eggert.  */
 
+#ifndef _@GUARD_PREFIX@_SYS_TIME_H_SEEN
+#define _@GUARD_PREFIX@_SYS_TIME_H_SEEN
+
 #ifndef _@GUARD_PREFIX@_SYS_TIME_H
 
 #if __GNUC__ >= 3
@@ -211,3 +214,5 @@
 #endif /* _@GUARD_PREFIX@_SYS_TIME_H */
 #endif /* _CYGWIN_SYS_TIME_H */
 #endif /* _@GUARD_PREFIX@_SYS_TIME_H */
+
+#endif /*_@GUARD_PREFIX@_SYS_TIME_H_SEEN */
