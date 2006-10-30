--- src/gui/sdl_mapper.cpp.orig	2006-10-27 16:19:56.437500000 +0100
+++ src/gui/sdl_mapper.cpp	2006-10-27 15:27:33.328125000 +0100
@@ -1634,7 +1634,13 @@
 	Section_prop * section=static_cast<Section_prop *>(sec);
 	usescancodes=false;
 
+#ifdef __riscos__
+	// Can't use scan code for RISC OS
+	if (0)
+	{
+#else
 	if (section->Get_bool("usescancodes")) {
+#endif
 		usescancodes=true;
 
 		/* Note: table has to be tested/updated for various OSs */
