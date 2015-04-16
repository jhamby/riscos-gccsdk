--- src/corelib/tools/qstring.h.orig	2015-02-17 04:56:49.000000000 +0000
+++ src/corelib/tools/qstring.h	2015-04-16 13:48:46.777520029 +0100
@@ -41,7 +41,7 @@
 
 #include <string>
 
-#if defined(Q_OS_ANDROID)
+#if defined(Q_OS_ANDROID) || defined(Q_OS_RISCOS)
 // std::wstring is disabled on android's glibc, as bionic lacks certain features
 // that libstdc++ checks for (like mbcslen).
 namespace std
