--- mono/utils/mono-codeman.c.orig	2011-08-09 02:18:46.000000000 +0100
+++ mono/utils/mono-codeman.c	2011-08-21 14:57:03.000000000 +0100
@@ -25,6 +25,13 @@
 #include <sys/nacl_syscalls.h>
 #endif
 
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
