--- gnu/link.c.orig	2012-04-23 13:39:07.000000000 +0100
+++ gnu/link.c	2012-04-23 13:41:37.000000000 +0100
@@ -144,6 +144,13 @@
   return 0;
 }
 
+#elif defined(__riscos__)
+int
+link (const char *file1, const char *file2)
+{
+  return 0;
+
+}
 # else /* !Windows */
 
 #  error "This platform lacks a link function, and Gnulib doesn't provide a replacement. This is a bug in Gnulib."
