--- mono/utils/mono-codeman.c.orig	2013-07-30 02:47:34.000000000 +0100
+++ mono/utils/mono-codeman.c	2013-08-16 12:40:13.888371471 +0100
@@ -29,6 +29,13 @@
 
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
