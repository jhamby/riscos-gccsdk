--- googletest/include/gtest/internal/gtest-port-arch.h.orig	2016-07-14 18:15:38.000000000 +0100
+++ googletest/include/gtest/internal/gtest-port-arch.h	2017-07-11 13:03:12.313009000 +0100
@@ -88,6 +88,8 @@
 # define GTEST_OS_OPENBSD 1
 #elif defined __QNX__
 # define GTEST_OS_QNX 1
+#elif defined __riscos__
+# define GTEST_OS_RISCOS 1
 #endif  // __CYGWIN__
 
 #endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PORT_ARCH_H_
