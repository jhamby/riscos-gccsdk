--- lib/fpending.c.orig	2020-09-07 23:55:36.483162168 +1200
+++ lib/fpending.c	2020-09-08 00:23:23.952047670 +1200
@@ -53,6 +53,8 @@
   return fp->wp - fp->buf;
 #elif defined __VMS                  /* VMS */
   return (*fp)->_ptr - (*fp)->_base;
+#elif defined __riscos__
+  return fp->o_ptr - fp->o_base;
 #else
 # error "Please port gnulib fpending.c to your platform!"
   return 1;
