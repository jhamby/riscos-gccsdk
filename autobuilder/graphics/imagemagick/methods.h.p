--- magick/methods.h.orig	2012-11-17 14:04:21.719023826 +0000
+++ magick/methods.h	2012-11-02 19:19:02.000000000 +0000
@@ -950,6 +950,7 @@
 #define RegisterSFWImage  PrependMagickMethod(RegisterSFWImage)
 #define RegisterSGIImage  PrependMagickMethod(RegisterSGIImage)
 #define RegisterStaticModules  PrependMagickMethod(RegisterStaticModules)
+#define RegisterSPRImage  PrependMagickMethod(RegisterSPRImage)
 #define RegisterSTEGANOImage  PrependMagickMethod(RegisterSTEGANOImage)
 #define RegisterSUNImage  PrependMagickMethod(RegisterSUNImage)
 #define RegisterSVGImage  PrependMagickMethod(RegisterSVGImage)
@@ -1280,6 +1281,7 @@
 #define UnregisterSFWImage  PrependMagickMethod(UnregisterSFWImage)
 #define UnregisterSGIImage  PrependMagickMethod(UnregisterSGIImage)
 #define UnregisterStaticModules  PrependMagickMethod(UnregisterStaticModules)
+#define UnregisterSPRImage  PrependMagickMethod(UnregisterSPRImage)
 #define UnregisterSTEGANOImage  PrependMagickMethod(UnregisterSTEGANOImage)
 #define UnregisterSUNImage  PrependMagickMethod(UnregisterSUNImage)
 #define UnregisterSVGImage  PrependMagickMethod(UnregisterSVGImage)
