--- Source_Files/RenderOther/screen_shared.h.orig	2009-08-13 00:30:34.000000000 +0100
+++ Source_Files/RenderOther/screen_shared.h	2009-12-11 11:21:17.848442700 +0000
@@ -56,7 +56,9 @@
 
 #include "network_games.h"
 #include "Image_Blitter.h"
+#ifdef HAVE_OPENGL
 #include "OGL_Blitter.h"
+#endif
 
 /* ---------- globals */
 
@@ -126,7 +128,9 @@
 	int color;
 	char text[Len];
 	Image_Blitter sdl_blitter;
+#ifdef HAVE_OPENGL
 	OGL_Blitter ogl_blitter;
+#endif
 } ScriptHUDElements[MAXIMUM_NUMBER_OF_SCRIPT_HUD_ELEMENTS];
 /* /SB */
 
@@ -238,11 +242,15 @@
 #else
 	srf = SDL_CreateRGBSurfaceFrom(ScriptHUDElements[idx].icon, 16, 16, 32, 64, 0xFF<<16, 0xFF<<8, 0xFF, 0xFF<<24);
 #endif
+#ifdef HAVE_OPENGL
 	if (OGL_IsActive()) {
 		ScriptHUDElements[idx].ogl_blitter.Load(*srf);
 	} else {
+#endif
 		ScriptHUDElements[idx].sdl_blitter.Load(*srf);
+#ifdef HAVE_OPENGL
 	}
+#endif
 	SDL_FreeSurface(srf);
   }
 	
