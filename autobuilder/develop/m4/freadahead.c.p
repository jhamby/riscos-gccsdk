--- lib/freadahead.c.orig	2017-01-01 02:54:41.000000000 +1300
+++ lib/freadahead.c	2020-09-08 00:45:58.397763420 +1200
@@ -85,6 +85,10 @@
   if (fp->state == 4 /* WR */ || fp->rp >= fp->wp)
     return 0;
   return fp->wp - fp->rp;
+#elif defined __riscos__
+  if (fp->__mode.__bits.__read != 0)
+    return fp->i_cnt;
+  return 0;  
 #elif defined SLOW_BUT_NO_HACKS     /* users can define this */
   abort ();
   return 0;
