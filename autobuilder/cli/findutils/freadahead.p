--- gl/lib/freadahead.c.orig	2020-11-30 15:01:05.915184730 +0000
+++ gl/lib/freadahead.c	2020-11-30 15:02:37.323937102 +0000
@@ -94,6 +94,10 @@
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
