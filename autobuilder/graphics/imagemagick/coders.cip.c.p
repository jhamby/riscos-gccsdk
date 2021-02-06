--- coders/cip.c.orig	2020-05-13 18:57:25.419805170 +0100
+++ coders/cip.c	2020-05-13 18:58:14.722820494 +0100
@@ -156,6 +156,7 @@
 %    o image:  The image.
 %
 */
+#pragma GCC optimize ("O1")
 static MagickBooleanType WriteCIPImage(const ImageInfo *image_info,Image *image)
 {
   char
