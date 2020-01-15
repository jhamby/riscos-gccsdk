--- src/gui/sdlmain.cpp.orig	2019-12-31 13:39:03.862737787 +0000
+++ src/gui/sdlmain.cpp	2020-01-14 19:47:45.896621625 +0000
@@ -1913,7 +1913,10 @@
 #endif
 	// Don't init timers, GetTicks seems to work fine and they can use a fair amount of power (Macs again) 
 	// Please report problems with audio and other things.
-	if ( SDL_Init( SDL_INIT_AUDIO|SDL_INIT_VIDEO | /*SDL_INIT_TIMER |*/ SDL_INIT_CDROM
+	if ( SDL_Init( SDL_INIT_AUDIO|SDL_INIT_VIDEO /*SDL_INIT_TIMER |*/ 
+#ifndef __riscos__
+		|SDL_INIT_CDROM
+#endif
 		|SDL_INIT_NOPARACHUTE
 		) < 0 ) E_Exit("Can't init SDL %s",SDL_GetError());
 	sdl.inited = true;
