--- Source_Files/RenderOther/Shape_Blitter.cpp.orig	2009-08-11 01:02:30.000000000 +0100
+++ Source_Files/RenderOther/Shape_Blitter.cpp	2009-12-11 11:18:04.428442700 +0000
@@ -27,9 +27,11 @@
 #include "shell.h"
 #include "scottish_textures.h"
 
+#ifdef HAVE_OPENGL
 #include "OGL_Setup.h"
 #include "OGL_Textures.h"
 #include "OGL_Blitter.h"
+#endif
 
 #ifdef HAVE_OPENGL
 # if defined (__APPLE__) && defined (__MACH__)
