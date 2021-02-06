--- magick/static.h.orig	2015-06-18 17:16:50.501897137 +0100
+++ magick/static.h	2015-06-18 17:40:27.729881754 +0100
@@ -153,6 +153,7 @@
   RegisterSGIImage(void),
   RegisterSHTMLImage(void),
   RegisterSIXELImage(void),
+  RegisterSPRImage(void),
   RegisterSTEGANOImage(void),
   RegisterSUNImage(void),
   RegisterSVGImage(void),
@@ -318,6 +319,7 @@
   UnregisterSGIImage(void),
   UnregisterSHTMLImage(void),
   UnregisterSIXELImage(void),
+  UnregisterSPRImage(void),
   UnregisterSTEGANOImage(void),
   UnregisterSUNImage(void),
   UnregisterSVGImage(void),
