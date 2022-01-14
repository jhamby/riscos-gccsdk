--- gio/gtrashportal.c.orig	2021-03-18 13:47:48.000000000 +0000
+++ gio/gtrashportal.c	2022-01-14 11:41:50.406329384 +0000
@@ -41,6 +41,12 @@
 #define O_PATH 0
 #endif
 
+#ifdef __riscos__
+#ifndef O_NOFOLLOW
+#define O_NOFOLLOW 0
+#endif
+#endif
+
 static GXdpTrash *
 ensure_trash_portal (void)
 {
