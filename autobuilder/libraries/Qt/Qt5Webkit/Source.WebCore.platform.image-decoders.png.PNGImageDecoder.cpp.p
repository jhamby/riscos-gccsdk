--- Source/WebCore/platform/image-decoders/png/PNGImageDecoder.cpp.orig	2015-02-17 04:57:06.000000000 +0000
+++ Source/WebCore/platform/image-decoders/png/PNGImageDecoder.cpp	2015-04-21 19:50:05.519337132 +0100
@@ -403,22 +403,36 @@
 
 static inline void setPixelRGB(ImageFrame::PixelData* dest, png_bytep pixel)
 {
+#if OS(RISCOS)
+    *dest = 0xFF000000U | pixel[2] << 16 | pixel[1] << 8 | pixel[0];
+#else
     *dest = 0xFF000000U | pixel[0] << 16 | pixel[1] << 8 | pixel[2];
+#endif
 }
 
 static inline void setPixelRGBA(ImageFrame::PixelData* dest, png_bytep pixel, unsigned char& nonTrivialAlphaMask)
 {
     unsigned char a = pixel[3];
+#if OS(RISCOS)
+    *dest = a << 24 | pixel[2] << 16 | pixel[1] << 8 | pixel[0];
+#else
     *dest = a << 24 | pixel[0] << 16 | pixel[1] << 8 | pixel[2];
+#endif
     nonTrivialAlphaMask |= (255 - a);
 }
 
 static inline void setPixelPremultipliedRGBA(ImageFrame::PixelData* dest, png_bytep pixel, unsigned char& nonTrivialAlphaMask)
 {
     unsigned char a = pixel[3];
+#if OS(RISCOS)
+    unsigned char r = fastDivideBy255(pixel[2] * a);
+    unsigned char g = fastDivideBy255(pixel[1] * a);
+    unsigned char b = fastDivideBy255(pixel[0] * a);
+#else
     unsigned char r = fastDivideBy255(pixel[0] * a);
     unsigned char g = fastDivideBy255(pixel[1] * a);
     unsigned char b = fastDivideBy255(pixel[2] * a);
+#endif
 
     *dest = a << 24 | r << 16 | g << 8 | b;
     nonTrivialAlphaMask |= (255 - a);
