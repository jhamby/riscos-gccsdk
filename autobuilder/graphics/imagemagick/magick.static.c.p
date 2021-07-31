--- magick/static.c.orig	2020-05-13 19:22:53.196963429 +0100
+++ magick/static.c	2020-05-14 19:31:41.720000000 +0100
@@ -202,6 +202,7 @@
   { "SFW", MagickFalse, RegisterSFWImage, UnregisterSFWImage },
   { "SGI", MagickFalse, RegisterSGIImage, UnregisterSGIImage },
   { "SIXEL", MagickFalse, RegisterSIXELImage, UnregisterSIXELImage },
+  { "SPR", MagickFalse, RegisterSPRImage, UnregisterSPRImage },
   { "STEGANO", MagickFalse, RegisterSTEGANOImage, UnregisterSTEGANOImage },
   { "SUN", MagickFalse, RegisterSUNImage, UnregisterSUNImage },
   { "SVG", MagickFalse, RegisterSVGImage, UnregisterSVGImage },
