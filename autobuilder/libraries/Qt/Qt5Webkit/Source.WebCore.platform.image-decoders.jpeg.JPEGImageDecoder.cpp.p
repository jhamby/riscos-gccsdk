--- Source/WebCore/platform/image-decoders/jpeg/JPEGImageDecoder.cpp.orig	2015-02-17 04:57:06.000000000 +0000
+++ Source/WebCore/platform/image-decoders/jpeg/JPEGImageDecoder.cpp	2015-04-21 19:47:33.611335164 +0100
@@ -641,7 +641,11 @@
 
     switch (colorSpace) {
     case JCS_RGB:
+#if OS(RISCOS)
+        buffer.setRGBA(currentAddress, jsample[2], jsample[1], jsample[0], 0xFF);
+#else
         buffer.setRGBA(currentAddress, jsample[0], jsample[1], jsample[2], 0xFF);
+#endif
         break;
     case JCS_CMYK:
         // Source is 'Inverted CMYK', output is RGB.
@@ -654,7 +658,11 @@
         // From CMY (0..1) to RGB (0..1):
         // R = 1 - C => 1 - (1 - iC*iK) => iC*iK  [G and B similar]
         unsigned k = jsample[3];
+#if OS(RISCOS)
+        buffer.setRGBA(currentAddress, jsample[2] * k / 255, jsample[1] * k / 255, jsample[0] * k / 255, 0xFF);
+#else
         buffer.setRGBA(currentAddress, jsample[0] * k / 255, jsample[1] * k / 255, jsample[2] * k / 255, 0xFF);
+#endif
         break;
     }
 }
