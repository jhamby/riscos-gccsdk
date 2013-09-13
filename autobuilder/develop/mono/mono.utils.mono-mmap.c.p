--- mono/utils/mono-mmap.c.orig	2013-07-30 02:47:34.000000000 +0100
+++ mono/utils/mono-mmap.c	2013-08-02 20:24:44.855899124 +0100
@@ -39,6 +39,10 @@
 #define MAP_32BIT 0
 #endif
 
+#ifdef __riscos__
+#include "riscos-memory.c"
+#endif
+
 typedef struct {
 	int size;
 	int pid;
@@ -443,8 +447,62 @@
 }
 #endif // __native_client__
 
-#else
+#elif defined (__riscos__)
+int
+mono_pagesize (void)
+{
+	static int saved_pagesize = 0;
+	if (saved_pagesize)
+		return saved_pagesize;
+	saved_pagesize = getpagesize ();
+	return saved_pagesize;
+}
+
+void*
+mono_valloc (void *addr, size_t length, int flags)
+{
+	int mflags = 0;
+
+	mflags |= MAP_ANONYMOUS;
+
+	return mono_riscos_mmap (length, mflags);
+}
+
+int
+mono_vfree (void *addr, size_t length)
+{
+	mono_riscos_munmap (addr, length);
+
+	return 0;
+}
+
+void*
+mono_file_map (size_t length, int flags, int fd, guint64 offset, void **ret_handle)
+{
+	void *ptr;
+	int mflags = 0;
+	int prot = 0;
+
+	ptr = mono_riscos_map_file (length, prot, mflags, fd, offset);
+	if (ptr == (void*)-1)
+		return NULL;
+	*ret_handle = (void*)length;
+	return ptr;
+}
 
+int
+mono_file_unmap (void *addr, void *handle)
+{
+	return mono_riscos_munmap (addr, (size_t)handle);
+}
+
+int
+mono_mprotect (void *addr, size_t length, int flags)
+{
+	return 0;
+}
+
+#else
 /* dummy malloc-based implementation */
 int
 mono_pagesize (void)
