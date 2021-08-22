--- src/corelib/tools/qstring.h.orig	2021-08-16 13:20:34.229132925 -0700
+++ src/corelib/tools/qstring.h	2021-08-16 13:39:11.571167506 -0700
@@ -56,7 +56,7 @@
 #include <string>
 #include <iterator>
 
-#if defined(Q_OS_ANDROID) && !defined(ANDROID_HAS_WSTRING)
+#if (defined(Q_OS_ANDROID) && !defined(ANDROID_HAS_WSTRING)) || defined(Q_OS_RISCOS)
 // std::wstring is disabled on android's glibc, as bionic lacks certain features
 // that libstdc++ checks for (like mbcslen).
 namespace std
