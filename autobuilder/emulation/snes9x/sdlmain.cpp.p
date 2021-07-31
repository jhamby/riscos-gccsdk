--- sdl/sdlmain.cpp.orig	2012-04-10 15:01:01.000000000 +0100
+++ sdl/sdlmain.cpp	2012-04-10 12:27:51.000000000 +0100
@@ -832,7 +832,7 @@
 	S9xReportControllers();
 
 #ifdef GFX_MULTI_FORMAT
-	S9xSetRenderPixelFormat(RGB565);
+	S9xSetRenderPixelFormat(BGR555);
 #endif
 
 	uint32	saved_flags = CPU.Flags;
