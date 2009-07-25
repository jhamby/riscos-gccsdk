--- lib/freadahead.c.orig	2009-05-03 22:46:33.000000000 -0700
+++ lib/freadahead.c	2009-07-25 08:53:49.000000000 -0700
@@ -76,6 +76,10 @@
   return (fp->__pushed_back
 	  ? fp->__get_limit - fp->__pushback_bufp + 1
 	  : fp->__get_limit - fp->__bufp);
+#elif defined __riscos__
+  if (fp->__mode.__bits.__read != 0)
+    return fp->i_cnt;
+  return 0;
 #elif defined SLOW_BUT_NO_HACKS     /* users can define this */
   abort ();
   return 0;
