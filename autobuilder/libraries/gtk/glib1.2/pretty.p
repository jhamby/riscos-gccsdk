--- glib.h.old	2004-09-13 14:19:32.000000000 +0100
+++ glib.h	2004-09-13 14:19:48.000000000 +0100
@@ -273,8 +273,8 @@
  * macros, so we can refer to them as strings unconditionally.
  */
 #ifdef	__GNUC__
-#define	G_GNUC_FUNCTION		__FUNCTION__
-#define	G_GNUC_PRETTY_FUNCTION	__PRETTY_FUNCTION__
+#define	G_GNUC_FUNCTION		""
+#define	G_GNUC_PRETTY_FUNCTION	""
 #else	/* !__GNUC__ */
 #define	G_GNUC_FUNCTION		""
 #define	G_GNUC_PRETTY_FUNCTION	""
