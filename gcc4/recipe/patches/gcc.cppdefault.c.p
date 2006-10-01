Index: gcc/cppdefault.c
===================================================================
RCS file: gcc/cppdefault.c,v
retrieving revision 1.9
diff -u -r1.9 cppdefault.c
--- gcc/cppdefault.c	3 Nov 2004 03:23:49 -0000	1.9
+++ gcc/cppdefault.c	17 May 2006 21:58:21 -0000
@@ -38,8 +38,12 @@
 # undef SYSTEM_INCLUDE_DIR
 # undef STANDARD_INCLUDE_DIR
 #else
+/* NAB++ */
+#ifndef TARGET_RISCOSAOF
 # undef CROSS_INCLUDE_DIR
 #endif
+/* NAB-- */
+#endif
 
 const struct default_include cpp_include_defaults[]
 #ifdef INCLUDE_DEFAULTS
@@ -96,3 +100,13 @@
 const char cpp_GCC_INCLUDE_DIR[] = "";
 const size_t cpp_GCC_INCLUDE_DIR_len = 0;
 #endif
+
+/* NAB++ */
+#ifdef CROSS_INCLUDE_DIR
+const char cpp_CROSS_INCLUDE_DIR[] = CROSS_INCLUDE_DIR;
+const size_t cpp_CROSS_INCLUDE_DIR_len = sizeof CROSS_INCLUDE_DIR;
+#else
+const char cpp_CROSS_INCLUDE_DIR[] = "";
+const size_t cpp_CROSS_INCLUDE_DIR_len = 0;
+#endif
+/* NAB-- */
