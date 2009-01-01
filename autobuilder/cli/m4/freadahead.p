--- lib/freadahead.c.orig	2009-01-01 11:15:29.000000000 -0800
+++ lib/freadahead.c	2009-01-01 11:26:09.000000000 -0800
@@ -78,6 +78,10 @@
 	 + (fp->_Mode & 0x4000 /* _MBYTE */
 	    ? (fp->_Back + sizeof (fp->_Back)) - fp->_Rback
 	    : 0);
+#elif defined __riscos__
+  if (fp->__mode.__bits.__write)
+    return 0;
+  return fp->i_cnt + (fp->__pushedback == 1);
 #else
  #error "Please port gnulib freadahead.c to your platform! Look at the definition of fflush, fread, ungetc on your system, then report this to bug-gnulib."
 #endif
