--- Source_Files/RenderMain/OGL_Setup.cpp.orig	2005-05-29 16:57:41.000000000 +0100
+++ Source_Files/RenderMain/OGL_Setup.cpp	2008-09-04 11:26:54.293000000 +0100
@@ -210,6 +210,7 @@
 	return (String.size() > 1);
 }
 
+#ifdef HAVE_OPENGL
 
 void OGL_TextureOptionsBase::Load()
 {
@@ -334,6 +335,7 @@
 	}
 }
 
+#endif
 
 #ifdef HAVE_OPENGL
 
@@ -366,11 +368,12 @@
 
 #else
 
-void OGL_LoadModelsImages(int Collection)
+/** ARB for RISC OS port, Change int to short in following functions */
+void OGL_LoadModelsImages(short Collection)
 {
 }
 
-void OGL_UnloadModelsImages(int Collection)
+void OGL_UnloadModelsImages(short Collection)
 {
 }
 
