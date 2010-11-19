--- src/gui/sdlmain.cpp.orig	2010-05-10 18:43:54.000000000 +0100
+++ src/gui/sdlmain.cpp	2010-05-25 20:20:32.000000000 +0100
@@ -1743,7 +1743,10 @@
 #if SDL_VERSION_ATLEAST(1, 2, 14)
 	putenv(const_cast<char*>("SDL_DISABLE_LOCK_KEYS=1"));
 #endif
-	if ( SDL_Init( SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_CDROM
+	if ( SDL_Init( SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER
+#ifndef __riscos__
+		|SDL_INIT_CDROM
+#endif
 		|SDL_INIT_NOPARACHUTE
 		) < 0 ) E_Exit("Can't init SDL %s",SDL_GetError());
 	sdl.inited = true;
