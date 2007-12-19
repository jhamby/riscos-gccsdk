--- src/lessons.c.orig	2007-12-13 12:16:56.593498900 +0000
+++ src/lessons.c	2007-12-13 12:20:44.815498900 +0000
@@ -120,12 +120,23 @@
   return;
 }
 
+/* RISC OS doesn't have a working version of basename, so
+ * add a simple version that will hopefully be enough.
+ */
+char *riscos_basename(const unsigned char *s)
+{
+   /* Assumming we are being passed a unix format filename */
+   char *leaf = strrchr(s, '/');
+   if (leaf == NULL) leaf = s;
+   return s;
+}
+
 /* Perform a strcasecmp() on two path strings, stripping away all the */
 /* dirs in the path and just comparing the filenames themselves:      */
 static int filename_comp(const unsigned char* s1, const unsigned char* s2)
 {
-  char* f1 = basename(s1);
-  char* f2 = basename(s2);
+  char* f1 = riscos_basename(s1);
+  char* f2 = riscos_basename(s2);
   return strcasecmp(f1, f2);
 }
 
