--- mono/utils/mono-mmap.h.orig	2012-04-10 20:47:48.000000000 +0100
+++ mono/utils/mono-mmap.h	2012-04-10 20:48:11.000000000 +0100
@@ -52,5 +52,10 @@
 
 void mono_file_map_set_allocator (mono_file_map_alloc_fn alloc, mono_file_map_release_fn release);
 				  
+#ifdef __riscos__
+void *mono_riscos_memory_alloc (size_t size);
+void mono_riscos_memory_free (void *block);
+#endif
+
 #endif /* __MONO_UTILS_MMAP_H__ */
 
