--- googletest/include/gtest/gtest-printers.h.orig	2016-07-14 18:15:38.000000000 +0100
+++ googletest/include/gtest/gtest-printers.h	2017-07-20 13:08:50.623008000 +0100
@@ -532,12 +532,14 @@
 // wchar_t* would cause unsigned short* be printed as a wide string,
 // possibly causing invalid memory accesses.
 #if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)
+#if !defined(GTEST_OS_RISCOS)
 // Overloads for wide C strings
 GTEST_API_ void PrintTo(const wchar_t* s, ::std::ostream* os);
 inline void PrintTo(wchar_t* s, ::std::ostream* os) {
   PrintTo(ImplicitCast_<const wchar_t*>(s), os);
 }
 #endif
+#endif
 
 // Overload for C arrays.  Multi-dimensional arrays are printed
 // properly.
