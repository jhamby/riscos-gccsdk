--- ./src/network/cookiejar/networkcookiejar/networkcookiejar.cpp.orig	2015-12-02 21:57:21.302982612 +0000
+++ ./src/network/cookiejar/networkcookiejar/networkcookiejar.cpp	2015-12-02 21:57:45.598982073 +0000
@@ -49,6 +49,8 @@
 
 #include <qurl.h>
 #include <qdatetime.h>
+#include <qiodevice.h>
+#include <qdatastream.h>
 
 #if defined(NETWORKCOOKIEJAR_DEBUG)
 #include <qdebug.h>
