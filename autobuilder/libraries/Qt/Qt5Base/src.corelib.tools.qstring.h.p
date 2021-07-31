--- src/corelib/tools/qstring.h.orig	2015-06-29 21:05:05.000000000 +0100
+++ src/corelib/tools/qstring.h	2015-07-15 18:24:20.218735900 +0100
@@ -45,7 +45,7 @@
 
 #include <string>
 
-#if defined(Q_OS_ANDROID)
+#if defined(Q_OS_ANDROID) || defined(Q_OS_RISCOS)
 // std::wstring is disabled on android's glibc, as bionic lacks certain features
 // that libstdc++ checks for (like mbcslen).
 namespace std
