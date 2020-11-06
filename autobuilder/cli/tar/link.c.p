--- gnu/link.c.orig	2018-04-30 05:45:06.000000000 +1200
+++ gnu/link.c	2020-10-09 11:40:49.120352181 +1300
@@ -141,6 +141,13 @@
   return 0;
 }
 
+# elif defined(__riscos__)
+int
+link (const char *file1, const char *file2)
+{
+  return 0;
+}
+
 # else /* !Windows */
 
 #  error "This platform lacks a link function, and Gnulib doesn't provide a replacement. This is a bug in Gnulib."
