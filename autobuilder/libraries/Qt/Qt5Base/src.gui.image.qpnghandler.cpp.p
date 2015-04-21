--- src/gui/image/qpnghandler.cpp.orig	2015-04-20 20:21:21.000000000 +0100
+++ src/gui/image/qpnghandler.cpp	2015-04-20 20:21:59.752256799 +0100
@@ -649,6 +649,11 @@
     if (scaledSize.isValid() && outImage->size() != scaledSize)
         *outImage = outImage->scaled(scaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
 
+#ifdef Q_OS_RISCOS
+    if (outImage->format() != QImage::Format_Indexed8)
+        *outImage = outImage->rgbSwapped();
+#endif
+
     return true;
 }
 
