--- sdl/sdlvideo.cpp.orig	2012-04-10 15:00:45.000000000 +0100
+++ sdl/sdlvideo.cpp	2012-04-10 12:27:44.000000000 +0100
@@ -173,7 +173,7 @@
 	 * we just go along with RGB565 for now, nothing else..
 	 */
 
-	S9xSetRenderPixelFormat(RGB565);
+	S9xSetRenderPixelFormat(BGR555);
 
 	S9xBlitFilterInit();
 	S9xBlit2xSaIFilterInit();
