--- Source/WebCore/platform/image-decoders/png/PNGImageDecoder.cpp.orig	2015-02-17 04:57:06.000000000 +0000
+++ Source/WebCore/platform/image-decoders/png/PNGImageDecoder.cpp	2015-05-08 15:13:32.420157656 +0100
@@ -403,12 +403,22 @@
 
 static inline void setPixelRGB(ImageFrame::PixelData* dest, png_bytep pixel)
 {
+#if OS(RISCOS)
+    if (QSysInfo::requireRedBlueSwap())
+	*dest = 0xFF000000U | pixel[2] << 16 | pixel[1] << 8 | pixel[0];
+    else
+#endif
     *dest = 0xFF000000U | pixel[0] << 16 | pixel[1] << 8 | pixel[2];
 }
 
 static inline void setPixelRGBA(ImageFrame::PixelData* dest, png_bytep pixel, unsigned char& nonTrivialAlphaMask)
 {
     unsigned char a = pixel[3];
+#if OS(RISCOS)
+    if (QSysInfo::requireRedBlueSwap())
+	*dest = a << 24 | pixel[2] << 16 | pixel[1] << 8 | pixel[0];
+    else
+#endif
     *dest = a << 24 | pixel[0] << 16 | pixel[1] << 8 | pixel[2];
     nonTrivialAlphaMask |= (255 - a);
 }
@@ -420,6 +430,11 @@
     unsigned char g = fastDivideBy255(pixel[1] * a);
     unsigned char b = fastDivideBy255(pixel[2] * a);
 
+#if OS(RISCOS)
+    if (QSysInfo::requireRedBlueSwap())
+	*dest = a << 24 | b << 16 | g << 8 | r;
+    else
+#endif
     *dest = a << 24 | r << 16 | g << 8 | b;
     nonTrivialAlphaMask |= (255 - a);
 }
