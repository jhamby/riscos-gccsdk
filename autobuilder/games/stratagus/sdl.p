--- ../orig/stratagus-040702/src/video/sdl.cpp	2004-06-27 09:38:20.000000000 +0100
+++ src/video/sdl.cpp	2005-01-11 12:53:58.000000000 +0000
@@ -257,6 +262,13 @@
 	ColorYellow = VideoMapRGB(TheScreen->format, 252, 252, 0);
 
 	UI.MouseWarpX = UI.MouseWarpY = -1;
+
+#ifdef __riscos__
+//ARB: Ensure NextFrameTicks has a sensible value at startup
+// as a random uninitialised value may cause the WaitEventsOneFrame
+// to wait for a very long time.
+      NextFrameTicks = SDL_GetTicks();
+#endif
 }
 
 /**
@@ -667,6 +679,12 @@
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
