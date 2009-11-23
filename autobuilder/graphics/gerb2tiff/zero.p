--- gerber.cpp.orig	2009-11-23 09:33:33.000000000 -0800
+++ gerber.cpp	2009-11-23 09:36:14.000000000 -0800
@@ -82,7 +82,7 @@
 */
 
 /*
- * Add a warning message to the general message string stream
+ * Add a warning message to the general message string stream
  */
 
 void Gerber::warning(char * format, ...)
@@ -577,7 +577,7 @@
 void Gerber::loadDefaults()
 {
 	isDrawingEnabled = false;
-	lastDrawnApertureSelect = 0;
+//	lastDrawnApertureSelect = 0;
 	isWarnNoApertureSelect = false;
 	layerPolarityClear = false;
 	scaleFactor[0] = optScaleX;
