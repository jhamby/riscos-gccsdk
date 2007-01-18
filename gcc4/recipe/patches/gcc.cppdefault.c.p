--- gcc/cppdefault.c.orig	2007-01-18 00:25:04.000000000 +0100
+++ gcc/cppdefault.c	2007-01-17 03:34:39.000000000 +0100
@@ -41,6 +41,10 @@
 # undef CROSS_INCLUDE_DIR
 #endif
 
+/* RISC OS : the CROSS_INCLUDE_DIR gets abused to select between UnixLib vs SCL headers.  */
+#undef CROSS_INCLUDE_DIR
+#define CROSS_INCLUDE_DIR TOOL_INCLUDE_DIR
+
 const struct default_include cpp_include_defaults[]
 #ifdef INCLUDE_DEFAULTS
 = INCLUDE_DEFAULTS;
@@ -96,3 +100,6 @@
 const char cpp_GCC_INCLUDE_DIR[] = "";
 const size_t cpp_GCC_INCLUDE_DIR_len = 0;
 #endif
+
+const char cpp_CROSS_INCLUDE_DIR[] = CROSS_INCLUDE_DIR;
+const size_t cpp_CROSS_INCLUDE_DIR_len = sizeof CROSS_INCLUDE_DIR;
