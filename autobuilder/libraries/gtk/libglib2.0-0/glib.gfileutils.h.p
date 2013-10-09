--- glib/gfileutils.h.orig	2013-05-07 19:26:07.000000000 +0100
+++ glib/gfileutils.h	2013-10-09 12:44:08.333926779 +0100
@@ -155,9 +155,19 @@
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
