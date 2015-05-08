--- src/gui/image/qpnghandler.cpp.orig	2015-02-17 04:56:48.000000000 +0000
+++ src/gui/image/qpnghandler.cpp	2015-05-08 16:42:09.440310138 +0100
@@ -400,6 +400,9 @@
 
     // Qt==ARGB==Big(ARGB)==Little(BGRA)
     if (QSysInfo::ByteOrder == QSysInfo::LittleEndian) {
+#ifdef Q_OS_RISCOS
+      if (!QSysInfo::requireRedBlueSwap())
+#endif
         png_set_bgr(png_ptr);
     }
 }
