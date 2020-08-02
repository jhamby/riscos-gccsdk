--- glib/gfileutils.h.orig	2014-02-03 17:40:41.000000000 +0000
+++ glib/gfileutils.h	2014-05-08 17:08:34.140137594 +0100
@@ -154,9 +154,19 @@
 #define G_DIR_SEPARATOR '/'
 #define G_DIR_SEPARATOR_S "/"
 #define G_IS_DIR_SEPARATOR(c) ((c) == G_DIR_SEPARATOR)
+
+#ifdef __riscos__
+
+/* Colon has special meaning in RISC OS pathnames, use comma instead. */
+#define G_SEARCHPATH_SEPARATOR ','
+#define G_SEARCHPATH_SEPARATOR_S ","
+
+#else  /* !__riscos__ */
+
 #define G_SEARCHPATH_SEPARATOR ':'
 #define G_SEARCHPATH_SEPARATOR_S ":"
 
+#endif /* !__riscos__ */
 #endif /* !G_OS_WIN32 */
 
 GLIB_AVAILABLE_IN_ALL
