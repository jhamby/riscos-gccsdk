--- Source_Files/RenderMain/OGL_Model_Def.cpp.orig	2003-05-15 16:16:16.000000000 +0100
+++ Source_Files/RenderMain/OGL_Model_Def.cpp	2008-09-04 11:26:54.171000000 +0100
@@ -27,6 +27,8 @@
 #include "cseries.h"
 #include "OGL_Model_Def.h"
 
+#ifdef HAVE_OPENGL
+
 #include <cmath>
 
 #include "Dim3_Loader.h"
@@ -959,3 +961,5 @@
 	return &ModelDataParser;
 }
 XML_ElementParser *Mdl_Clear_GetParser() {return &Mdl_ClearParser;}
+
+#endif
