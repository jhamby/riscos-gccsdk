--- src/mod_dirlisting.c.orig	2022-11-30 13:06:26.775134405 +0000
+++ src/mod_dirlisting.c	2022-11-30 13:08:35.031262832 +0000
@@ -35,6 +35,10 @@
 #define _ATFILE_SOURCE
 #endif
 #endif
+#ifdef __riscos__
+#undef AT_FDCWD
+#undef _ATFILE_SOURCE
+#endif
 
 #ifndef _D_EXACT_NAMLEN
 #ifdef _DIRENT_HAVE_D_NAMLEN
