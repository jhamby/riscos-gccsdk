Index: gcc/cppdefault.h
===================================================================
RCS file: gcc/cppdefault.h,v
retrieving revision 1.10
diff -u -r1.10 cppdefault.h
--- gcc/cppdefault.h	3 Nov 2004 03:23:49 -0000	1.10
+++ gcc/cppdefault.h	17 May 2006 21:58:21 -0000
@@ -49,4 +49,9 @@
 extern const char cpp_GCC_INCLUDE_DIR[];
 extern const size_t cpp_GCC_INCLUDE_DIR_len;
 
+/* NAB++ */
+extern const char cpp_CROSS_INCLUDE_DIR[];
+extern const size_t cpp_CROSS_INCLUDE_DIR_len;
+/* NAB-- */
+
 #endif /* ! GCC_CPPDEFAULT_H */
