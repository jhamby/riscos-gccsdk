--- include/git2/common.h.orig	2018-11-10 11:25:05.987216139 +0000
+++ include/git2/common.h	2018-11-10 11:25:35.890900716 +0000
@@ -59,6 +59,10 @@
 #define GIT_WIN32 1
 #endif
 
+#if defined(__riscos__)
+#define GIT_RISCOS 1
+#endif
+
 #ifdef __amigaos4__
 #include <netinet/in.h>
 #endif
@@ -84,6 +84,8 @@
  */
 #ifdef GIT_WIN32
 #define GIT_PATH_LIST_SEPARATOR ';'
+#elif defined(GIT_RISCOS)
+#define GIT_PATH_LIST_SEPARATOR ','
 #else
 #define GIT_PATH_LIST_SEPARATOR ':'
 #endif
