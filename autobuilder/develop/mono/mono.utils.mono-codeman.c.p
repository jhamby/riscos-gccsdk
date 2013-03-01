--- mono/utils/mono-codeman.c.orig	2013-01-08 18:41:09.000000000 +0000
+++ mono/utils/mono-codeman.c	2013-01-16 19:41:47.000000000 +0000
@@ -28,6 +28,13 @@
 
 static uintptr_t code_memory_used = 0;
 
+#if defined(__riscos__)
+/* Malloc allocations are already word aligned.  */
+#define FORCE_MALLOC
+#define dlmemalign(align, size) malloc(size)
+#define dlfree free
+#endif
+
 /*
  * AMD64 processors maintain icache coherency only for pages which are 
  * marked executable. Also, windows DEP requires us to obtain executable memory from
