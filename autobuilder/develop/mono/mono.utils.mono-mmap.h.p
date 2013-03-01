--- mono/utils/mono-mmap.h.orig	2013-01-08 18:41:09.000000000 +0000
+++ mono/utils/mono-mmap.h	2013-01-29 20:15:22.000000000 +0000
@@ -57,5 +57,13 @@
 
 void mono_file_map_set_allocator (mono_file_map_alloc_fn alloc, mono_file_map_release_fn release);
 				  
+#ifdef __riscos__
+void *mono_riscos_mmap (size_t size, int flags);
+void *mono_riscos_memory_alloc (size_t size);
+#define mono_riscos_munmap mono_riscos_memory_free
+int mono_riscos_memory_free (void *addr, size_t len);
+void *mono_riscos_map_file (size_t length, int prot, int flags, int fd, off_t offset);
+#endif
+
 #endif /* __MONO_UTILS_MMAP_H__ */
 
