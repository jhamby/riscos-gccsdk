--- src/gui/sdl_mapper.cpp.orig	2010-05-10 19:58:06.000000000 +0100
+++ src/gui/sdl_mapper.cpp	2010-11-18 23:46:16.000000000 +0000
@@ -2397,7 +2397,13 @@
 
 	usescancodes = false;
 
+#ifdef __riscos__
+	// Can't use scan code for RISC OS
+	if (0)
+	{
+#else
 	if (section->Get_bool("usescancodes")) {
+#endif
 		usescancodes=true;
 
 		/* Note: table has to be tested/updated for various OSs */

