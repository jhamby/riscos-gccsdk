--- FL/x.H.orig	2012-11-22 14:42:03.294656519 +0000
+++ FL/x.H	2012-11-22 14:42:28.934575341 +0000
@@ -84,7 +84,7 @@
 private:
   XFontStruct* ptr;
 };
-extern FL_EXPORT Fl_XFont_On_Demand fl_xfont;
+extern XFontStruct* fl_xfont;

 // drawing functions:
 extern FL_EXPORT GC fl_gc;
