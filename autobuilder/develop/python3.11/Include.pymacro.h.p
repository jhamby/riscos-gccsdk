--- Include/pymacro.h.orig	2023-07-01 11:11:23.655630876 +0100
+++ Include/pymacro.h	2023-07-01 11:11:40.906910719 +0100
@@ -10,6 +10,9 @@
 #  define static_assert _Static_assert
 #endif
 
+#if defined(__riscos__) && !defined(static_assert)
+#  define static_assert _Static_assert
+#endif
 // static_assert is defined in glibc from version 2.16. Before it requires
 // compiler support (gcc >= 4.6) and is called _Static_assert.
 // In C++ 11 static_assert is a keyword, redefining is undefined behaviour.
