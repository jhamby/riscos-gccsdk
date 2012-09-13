--- glib/gutils.h.orig	2009-12-01 04:25:05.000000000 +0000
+++ glib/gutils.h	2009-12-27 15:00:29.000000000 +0000
@@ -55,9 +55,19 @@
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
+#else /* !__riscos__ */
+
 #define G_SEARCHPATH_SEPARATOR ':'
 #define G_SEARCHPATH_SEPARATOR_S ":"
 
+#endif /* __riscos__ */
 #endif /* !G_OS_WIN32 */
 
 /* Define G_VA_COPY() to do the right thing for copying va_list variables.
