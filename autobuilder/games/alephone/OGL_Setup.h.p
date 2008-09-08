--- Source_Files/RenderMain/OGL_Setup.h.orig	2003-09-14 20:58:42.000000000 +0100
+++ Source_Files/RenderMain/OGL_Setup.h	2008-09-04 11:26:54.420000000 +0100
@@ -184,8 +184,9 @@
 
 // Does this for a set of several pixel values or color-table values;
 // the pixels are assumed to be in OpenGL-friendly byte-by-byte RGBA format.
+#ifdef HAVE_OPENGL
 void SetPixelOpacities(OGL_TextureOptions& Options, int NumPixels, uint32 *Pixels);
-
+#endif
 
 
 // Reset the textures (walls, sprites, and model skins) (good if they start to crap out)
