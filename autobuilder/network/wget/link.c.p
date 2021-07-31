--- lib/link.c.orig	2017-02-05 03:41:52.000000000 +1300
+++ lib/link.c	2017-07-27 14:46:03.872408715 +1200
@@ -142,9 +142,21 @@
 }
 
 # else /* !Windows */
-
-#  error "This platform lacks a link function, and Gnulib doesn't provide a replacement. This is a bug in Gnulib."
-
+/* The system does not support links.(Copied from symlink.c)*/
+int
+link (char const *contents _GL_UNUSED,
+         char const *name _GL_UNUSED)
+{
+  errno = EPERM;
+  return -1;
+}
+int
+rpl_link (char const *contents _GL_UNUSED,
+         char const *name _GL_UNUSED)
+{
+  errno = EPERM;
+  return -1;
+}
 # endif /* !Windows */
 #else /* HAVE_LINK */
 
