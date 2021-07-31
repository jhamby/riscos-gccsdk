--- lib/freadahead.c.orig	2015-02-19 12:52:16.529992764 +0000
+++ lib/freadahead.c	2015-02-27 13:02:16.710974683 +0000
@@ -84,6 +84,10 @@
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
