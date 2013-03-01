--- mono/utils/mono-filemap.c.orig	2013-01-08 18:41:09.000000000 +0000
+++ mono/utils/mono-filemap.c	2013-01-29 20:29:27.000000000 +0000
@@ -97,7 +97,7 @@
 	(*release_fn) (addr);
 	return 0;
 }
-#if !defined(HAVE_MMAP)
+#if !defined(HAVE_MMAP) && !defined (__riscos__)
 void *
 mono_file_map (size_t length, int flags, int fd, guint64 offset, void **ret_handle)
 {
