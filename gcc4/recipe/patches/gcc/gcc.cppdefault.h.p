--- gcc/cppdefault.h.orig	2007-01-18 00:25:15.000000000 +0100
+++ gcc/cppdefault.h	2007-01-17 03:33:47.000000000 +0100
@@ -49,4 +49,7 @@
 extern const char cpp_GCC_INCLUDE_DIR[];
 extern const size_t cpp_GCC_INCLUDE_DIR_len;
 
+extern const char cpp_CROSS_INCLUDE_DIR[];
+extern const size_t cpp_CROSS_INCLUDE_DIR_len;
+
 #endif /* ! GCC_CPPDEFAULT_H */
