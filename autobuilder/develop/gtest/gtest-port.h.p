--- include/gtest/internal/gtest-port.h.orig	2015-09-18 08:25:18.781012000 +0100
+++ include/gtest/internal/gtest-port.h	2015-09-18 08:43:46.529050672 +0100
@@ -270,6 +270,8 @@
 # define GTEST_OS_OPENBSD 1
 #elif defined __QNX__
 # define GTEST_OS_QNX 1
+#elif defined __riscos__
+# define GTEST_OS_RISCOS 1
 #endif  // __CYGWIN__
 
 #ifndef GTEST_LANG_CXX11
@@ -398,7 +400,7 @@
 // Solaris' libc++ doesn't support it either.  Android has
 // no support for it at least as recent as Froyo (2.2).
 # define GTEST_HAS_STD_WSTRING \
-    (!(GTEST_OS_LINUX_ANDROID || GTEST_OS_CYGWIN || GTEST_OS_SOLARIS))
+    (!(GTEST_OS_LINUX_ANDROID || GTEST_OS_CYGWIN || GTEST_OS_SOLARIS || GTEST_OS_RISCOS))
 
 #endif  // GTEST_HAS_STD_WSTRING
 
