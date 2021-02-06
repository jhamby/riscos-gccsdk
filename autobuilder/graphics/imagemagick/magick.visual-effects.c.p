--- magick/visual-effects.c.orig	2020-05-17 07:59:45.374939103 +0100
+++ magick/visual-effects.c	2020-05-17 08:00:55.621712176 +0100
@@ -742,6 +742,7 @@
 %    o exception: return any errors or warnings in this structure.
 %
 */
+#pragma GCC optimize("O1")
 MagickExport Image *ColorMatrixImage(const Image *image,
   const KernelInfo *color_matrix,ExceptionInfo *exception)
 {
@@ -938,6 +939,7 @@
     color_image=DestroyImage(color_image);
   return(color_image);
 }
+#pragma GCC optimize("O3")
 
 /*
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
