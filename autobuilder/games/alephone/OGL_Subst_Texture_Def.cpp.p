--- Source_Files/RenderMain/OGL_Subst_Texture_Def.cpp.orig	2005-05-29 18:41:50.000000000 +0100
+++ Source_Files/RenderMain/OGL_Subst_Texture_Def.cpp	2008-09-04 11:26:54.569000000 +0100
@@ -28,6 +28,7 @@
 #include "cseries.h"
 #include "OGL_Subst_Texture_Def.h"
 
+#ifdef HAVE_OPENGL
 
 // Defined in OGL_Setup.cpp
 extern void SetPixelOpacities(OGL_TextureOptions& Options, int NumPixels, uint32 *Pixels);
@@ -375,3 +376,5 @@
 // XML-parser support:
 XML_ElementParser *TextureOptions_GetParser() {return &TextureOptionsParser;}
 XML_ElementParser *TO_Clear_GetParser() {return &TO_ClearParser;}
+
+#endif
