--- googletest/include/gtest/internal/gtest-port.h.orig	2016-07-14 18:15:38.000000000 +0100
+++ googletest/include/gtest/internal/gtest-port.h	2017-07-13 13:17:22.195009000 +0100
@@ -519,7 +519,7 @@
 // Solaris' libc++ doesn't support it either.  Android has
 // no support for it at least as recent as Froyo (2.2).
 # define GTEST_HAS_STD_WSTRING \
-    (!(GTEST_OS_LINUX_ANDROID || GTEST_OS_CYGWIN || GTEST_OS_SOLARIS))
+    (!(GTEST_OS_LINUX_ANDROID || GTEST_OS_CYGWIN || GTEST_OS_SOLARIS || GTEST_OS_RISCOS))
 
 #endif  // GTEST_HAS_STD_WSTRING
 
