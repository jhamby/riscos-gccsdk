--- include/compiler.h.orig	2010-09-29 20:11:37.000000000 +0100
+++ include/compiler.h	2010-09-29 20:11:37.000000000 +0100
@@ -25,6 +25,11 @@
 #include <stdio.h>
 #include <string.h>
 
+#if defined(__riscos__)
+# define mmap fake_mmap
+# define munmap fake_munmap
+#endif
+ 
 #if !defined(__WIN32__) && !defined(__MINGW32__)
 # include <sys/mman.h>
 #endif
