--- googletest/src/gtest-printers.cc.orig	2016-07-14 18:15:38.000000000 +0100
+++ googletest/src/gtest-printers.cc	2017-07-13 13:23:14.023009000 +0100
@@ -333,6 +333,7 @@
 // memory accesses. MSVC defines _NATIVE_WCHAR_T_DEFINED symbol when
 // wchar_t is implemented as a native type.
 #if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
+#if !defined(GTEST_OS_RISCOS)
 // Prints the given wide C string to the ostream.
 void PrintTo(const wchar_t* s, ostream* os) {
   if (s == NULL) {
@@ -342,6 +343,7 @@
     PrintCharsAsStringTo(s, std::wcslen(s), os);
   }
 }
+#endif
 #endif  // wchar_t is native
 
 // Prints a ::string object.
