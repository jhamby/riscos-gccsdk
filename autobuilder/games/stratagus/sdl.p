--- src/video/sdl.cpp.orig	2006-12-22 20:56:29.000000000 -0800
+++ src/video/sdl.cpp	2009-08-23 08:23:05.000000000 -0700
@@ -603,6 +603,12 @@
 		// The event handling of SDL is wrong designed = polling only.
 		// There is hope on SDL 1.3 which will have this fixed.
 
+#ifdef __riscos__
+// The version of select in RISC OS is relatively slow, so avoid calling
+// it when not running a network game
+            if (!IsNetworkGame()) s = 0;
+            else
+#endif
 		s = select(maxfd + 1, &rfds, &wfds, NULL, &tv);
 		i = SDL_PollEvent(event);
 #endif
