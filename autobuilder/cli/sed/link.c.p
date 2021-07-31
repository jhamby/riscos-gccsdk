--- gnulib-tests/link.c.orig	2021-01-13 19:56:51.441154520 +0000
+++ gnulib-tests/link.c	2021-01-13 19:58:42.122959532 +0000
@@ -145,7 +145,7 @@
   return 0;
 }
 
-# else /* !Windows */
+# elif !(defined __riscos__) /* !Windows */
 
 #  error "This platform lacks a link function, and Gnulib doesn't provide a replacement. This is a bug in Gnulib."
 
