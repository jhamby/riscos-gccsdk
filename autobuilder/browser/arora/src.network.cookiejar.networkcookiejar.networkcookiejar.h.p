--- src/network/cookiejar/networkcookiejar/networkcookiejar.h.orig	2015-04-10 20:47:29.380351907 +0100
+++ src/network/cookiejar/networkcookiejar/networkcookiejar.h	2015-04-11 14:01:54.685978347 +0100
@@ -38,6 +38,7 @@
 #define NETWORKCOOKIEJAR_H
 
 #include <qnetworkcookie.h>
+#include <qnetworkcookiejar.h>
 
 class NetworkCookieJarPrivate;
 class NetworkCookieJar : public QNetworkCookieJar {
