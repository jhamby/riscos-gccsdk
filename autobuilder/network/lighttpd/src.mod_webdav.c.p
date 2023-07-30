--- src/mod_webdav.c.orig	2022-11-30 13:06:07.444348760 +0000
+++ src/mod_webdav.c	2022-11-30 13:07:23.795597008 +0000
@@ -188,6 +188,10 @@
 #define _ATFILE_SOURCE
 #endif
 #endif
+#ifdef __riscos__
+#undef AT_FDCWD
+#undef _ATFILE_SOURCE
+#endif
 
 #ifndef AT_SYMLINK_NOFOLLOW
 #define AT_SYMLINK_NOFOLLOW 0
