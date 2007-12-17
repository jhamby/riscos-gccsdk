--- gcc/c-incpath.c.orig	2007-12-18 00:14:24.000000000 +0100
+++ gcc/c-incpath.c	2007-12-17 23:57:01.000000000 +0100
@@ -340,6 +340,20 @@
     if (*c == '\\') *c = '/';
 #endif
 
+#ifdef __riscos__
+  char *temp;
+  temp = __unixify_std (path, NULL, 0, 0, NULL);
+  if (!temp)
+    {
+      fprintf (stderr, _("Unixify failed (%s), falling back to input path\n"),
+      path);
+    }
+  else
+    {
+      free(path);
+      path = temp;
+    }
+#endif
   p = xmalloc (sizeof (cpp_dir));
   p->next = NULL;
   p->name = path;
