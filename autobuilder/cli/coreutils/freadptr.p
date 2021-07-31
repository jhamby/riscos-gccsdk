--- lib/freadptr.c.orig	2015-04-29 21:11:40.252379201 +0100
+++ lib/freadptr.c	2015-04-29 21:16:53.440385669 +0100
@@ -108,6 +108,14 @@
     return NULL;
   *sizep = fp->wp - fp->rp;
   return fp->rp;
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
