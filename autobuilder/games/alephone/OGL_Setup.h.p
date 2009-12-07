--- Source_Files/RenderMain/OGL_Textures.h.orig	2009-12-07 15:17:06.000000000 -0800
+++ Source_Files/RenderMain/OGL_Textures.h	2009-12-07 15:17:38.000000000 -0800
@@ -313,7 +313,9 @@
 
 // Does this for a set of several pixel values or color-table values;
 // the pixels are assumed to be in OpenGL-friendly byte-by-byte RGBA format.
+#ifdef HAVE_OPENGL
 void SetPixelOpacitiesRGBA(OGL_TextureOptions& Options, int NumPixels, uint32 *Pixels);
+#endif
 
 // Infravision (I'm blue, are you?)
 bool& IsInfravisionActive();
