--- src/corelib/codecs/qlatincodec.cpp.orig	2013-11-17 16:39:52.000000000 +0000
+++ src/corelib/codecs/qlatincodec.cpp	2013-11-17 16:40:38.748776863 +0000
@@ -114,7 +114,7 @@
     QString str = QString::fromLatin1(chars, len);
     QChar *uc = str.data();
     while(len--) {
-        switch(uc->unicode()) {
+        switch(RISCOS_DEREF_QCHAR(uc->unicode())) {
             case 0xa4:
                 *uc = 0x20ac;
                 break;
