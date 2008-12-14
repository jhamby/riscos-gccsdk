--- gcc/c-incpath.c.orig	2008-12-13 20:14:12.000000000 +0100
+++ gcc/c-incpath.c	2008-12-13 21:40:24.000000000 +0100
@@ -340,6 +340,21 @@ add_path (char *path, int chain, int cxx
     if (*c == '\\') *c = '/';
 #endif
 
+#ifdef __riscos__
+  extern char *__unixify_std (const char *, char *, size_t, int);
+  char *temp;
+  temp = __unixify_std (path, NULL, 0, 0);
+  if (!temp)
+    {
+      fprintf (stderr, _("Unixify failed (%s), falling back to input path\n"),
+	       path);
+    }
+  else
+    {
+      free (path);
+      path = temp;
+    }
+#endif
   p = xmalloc (sizeof (cpp_dir));
   p->next = NULL;
   p->name = path;
