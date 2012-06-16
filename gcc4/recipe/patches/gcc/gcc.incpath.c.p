Index: gcc/incpath.c
===================================================================
--- gcc/incpath.c	(revision 183662)
+++ gcc/incpath.c	(working copy)
@@ -416,6 +416,21 @@
     *end = 0;
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
   p = XNEW (cpp_dir);
   p->next = NULL;
   p->name = path;
