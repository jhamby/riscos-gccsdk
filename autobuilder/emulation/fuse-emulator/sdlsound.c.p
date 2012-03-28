--- sound/sdlsound.c.orig	2011-07-17 14:06:47.000000000 +0100
+++ sound/sdlsound.c	2011-07-17 13:49:50.000000000 +0100
@@ -94,7 +94,11 @@
      speed to about 2000% on my Mac, 100Hz allows up to 5000% for me) */
   if( hz > 100.0 ) hz = 100.0;
   sound_framesiz = *freqptr / hz;
+#ifndef __riscos__
   requested.samples = sound_framesiz;
+#else
+  requested.samples = 512;
+#endif

   if ( SDL_OpenAudio( &requested, &received ) < 0 ) {
     settings_current.sound = 0;
