--- lib/freadptr.c.orig	2009-07-25 09:04:48.000000000 -0700
+++ lib/freadptr.c	2009-07-25 09:10:51.000000000 -0700
@@ -93,6 +93,14 @@
     return NULL;
   *sizep = size;
   return fp->__bufp;
+#elif defined __riscos__
+  if (fp->__mode.__bits.__read != 0)
+    return NULL;
+  size = fp->__bufsize - (fp->i_ptr - fp->i_base);
+  if (size == 0)
+    return NULL;
+  *sizep = size;
+  return fp->i_ptr;
 #elif defined SLOW_BUT_NO_HACKS     /* users can define this */
   /* This implementation is correct on any ANSI C platform.  It is just
      awfully slow.  */
