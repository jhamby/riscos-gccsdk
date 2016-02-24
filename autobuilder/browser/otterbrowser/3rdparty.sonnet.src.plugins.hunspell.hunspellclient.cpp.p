--- 3rdparty/sonnet/src/plugins/hunspell/hunspellclient.cpp.orig	2016-02-24 21:00:04.890703873 +0000
+++ 3rdparty/sonnet/src/plugins/hunspell/hunspellclient.cpp	2016-02-24 21:01:02.630703932 +0000
@@ -56,6 +56,8 @@
 
 #ifdef Q_OS_MAC
     directories << QLatin1String("/System/Library/Spelling/");
+#elif defined Q_OS_RISCOS
+    directories << QLatin1String("/<Hunspell$Dir>/");
 #else
     directories << QLatin1String("/usr/share/hunspell/") << QLatin1String("/usr/local/share/hunspell/");
 #endif
