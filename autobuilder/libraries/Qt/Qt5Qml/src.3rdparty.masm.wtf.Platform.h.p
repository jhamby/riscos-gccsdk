--- src/3rdparty/masm/wtf/Platform.h.orig	2021-08-02 14:01:35.512064399 -0700
+++ src/3rdparty/masm/wtf/Platform.h	2021-08-02 14:03:37.444765779 -0700
@@ -395,6 +395,11 @@
 #define WTF_OS_QNX 1
 #endif
 
+/* OS(RISCOS) - RISC OS */
+#if defined(__riscos__)
+#define WTF_OS_RISCOS 1
+#endif
+
 /* OS(SOLARIS) - Solaris */
 #if defined(sun) || defined(__sun)
 #define WTF_OS_SOLARIS 1
@@ -427,6 +432,7 @@
     || OS(NETBSD)           \
     || OS(OPENBSD)          \
     || OS(QNX)              \
+    || OS(RISCOS)           \
     || OS(SOLARIS)          \
     || defined(unix)        \
     || defined(__unix)      \
