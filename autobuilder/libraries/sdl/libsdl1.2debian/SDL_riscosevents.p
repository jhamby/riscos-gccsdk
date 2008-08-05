--- src/video/riscos/SDL_riscosevents.c.orig	2007-12-31 03:48:02.000000000 +0000
+++ src/video/riscos/SDL_riscosevents.c	2008-07-25 16:05:10.308928500 +0100
@@ -44,6 +44,10 @@
 #include "kernel.h"
 #include "swis.h"
 
+#if !SDL_THREADS_DISABLED
+#include <pthread.h>
+#endif
+
 /* The translation table from a RISC OS internal key numbers to a SDL keysym */
 static SDLKey RO_keymap[SDLK_LAST];
 
@@ -83,6 +87,9 @@
 /* Timer running function */
 extern void RISCOS_CheckTimer();
 
+#else
+extern int riscos_using_threads;
+
 #endif
 
 void FULLSCREEN_PumpEvents(_THIS)
@@ -93,6 +100,14 @@
 #if SDL_THREADS_DISABLED
 //	DRenderer_FillBuffers();
 	if (SDL_timer_running) RISCOS_CheckTimer();
+#else
+	/* Stop thread starvation, which will occur if the main loop
+         doesn't call SDL_Delay */
+	if (riscos_using_threads)
+	{
+		pthread_yield();
+	}
+
 #endif
 }
 
