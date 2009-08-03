--- gnulib/lib/freadahead.c.orig	2009-05-10 14:23:57.000000000 -0700
+++ gnulib/lib/freadahead.c	2009-08-02 20:32:27.000000000 -0700
@@ -60,6 +60,10 @@
     return 0;
   /* fp->_Buf <= fp->_Next <= fp->_Rend */
   return fp->_Rend - fp->_Next;
+#elif defined __riscos__
+  if (fp->__mode.__bits.__read != 0)
+    return fp->i_cnt;
+  return 0;
 #else
  #error "Please port gnulib freadahead.c to your platform! Look at the definition of fflush, fread on your system, then report this to bug-gnulib."
 #endif
