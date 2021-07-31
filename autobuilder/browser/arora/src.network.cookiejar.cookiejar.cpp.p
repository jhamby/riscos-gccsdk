--- src/network/cookiejar/cookiejar.cpp.orig	2015-12-02 21:39:36.331006248 +0000
+++ src/network/cookiejar/cookiejar.cpp	2015-12-02 21:40:06.611005576 +0000
@@ -70,6 +70,8 @@
 #include <qmetaobject.h>
 #include <qsettings.h>
 #include <qurl.h>
+#include <qiodevice.h>
+#include <qdatastream.h>

 #include <qdebug.h>
 
