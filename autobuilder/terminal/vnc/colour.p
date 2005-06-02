--- vncviewer/colour.c.orig	2005-06-01 13:26:40.000000000 +0100
+++ vncviewer/colour.c	2005-06-01 13:31:55.000000000 +0100
@@ -274,7 +274,7 @@
  * for the given depth.
  */
 
-static int GetBPPForDepth(int depth)
+static int GetBPPForDepthMaybe(int depth)
 {
   XPixmapFormatValues *format;
   int nformats;
