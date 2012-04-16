--- mono/utils/mono-filemap.c.orig	2011-12-19 21:10:27.000000000 +0000
+++ mono/utils/mono-filemap.c	2012-04-15 17:34:15.000000000 +0100
@@ -77,7 +77,11 @@
 {
 	guint64 cur_offset;
 	size_t bytes_read;
+#ifdef __riscos__
+	void *ptr = mono_riscos_memory_alloc (length);
+#else
 	void *ptr = (*alloc_fn) (length);
+#endif
 	if (!ptr)
 		return NULL;
 	cur_offset = lseek (fd, 0, SEEK_CUR);
@@ -94,7 +98,11 @@
 int
 mono_file_unmap (void *addr, void *handle)
 {
+#ifdef __riscos__
+	mono_riscos_memory_free (addr);
+#else
 	(*release_fn) (addr);
+#endif
 	return 0;
 }
 #endif
