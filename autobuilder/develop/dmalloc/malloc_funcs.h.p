--- malloc_funcs.h.org	2004-10-19 16:51:21.000000000 +0200
+++ malloc_funcs.h	2006-11-12 23:36:19.000000000 +0100
@@ -148,6 +148,15 @@
  */
 extern
 char	*strdup(const char *str);
+
+/*
+ * Allocate and return a block of bytes that contains the string STR
+ * including the \0.
+ *
+ * Returns 0L on error.
+ */
+extern
+char	*strndup(const char *str, DMALLOC_SIZE size);
 #endif /* ifndef DMALLOC_STRDUP_MACRO */
 
 /*
