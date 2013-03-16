--- lib/link.c.orig	2010-06-08 18:08:18.532156682 +0100
+++ lib/link.c	2010-06-08 18:07:17.419662095 +0100
@@ -144,7 +144,7 @@
   return 0;
 }
 
-# else /* !Windows */
+# elif !(defined __riscos__) /* !Windows */
 
 #  error "This platform lacks a link function, and Gnulib doesn't provide a replacement. This is a bug in Gnulib."
 
