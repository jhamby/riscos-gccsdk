--- src/file.h.orig	2008-07-26 08:03:55.000000000 -0700
+++ src/file.h	2008-12-27 18:42:01.000000000 -0800
@@ -60,6 +60,8 @@

 #ifdef __EMX__
 #define PATHSEP	';'
+#elif __riscos__
+#define PATHSEP ','
 #else
 #define PATHSEP	':'
 #endif
@@ -398,7 +400,7 @@
 #endif

 #if defined(HAVE_MMAP) && defined(HAVE_SYS_MMAN_H) && !defined(QUICK)
-#define QUICK
+//#define QUICK
 #endif

 #ifndef O_BINARY
