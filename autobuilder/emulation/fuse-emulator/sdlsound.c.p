--- sound/sdlsound.c.orig	2021-03-14 11:55:32.972326378 +0000
+++ sound/sdlsound.c	2021-03-14 11:58:24.346233658 +0000
@@ -93,7 +93,7 @@
      speed to about 2000% on my Mac, 100Hz allows up to 5000% for me) */
   if( hz > 100.0 ) hz = 100.0;
   sound_framesiz = *freqptr / hz;
-#ifdef __FreeBSD__
+#if defined  __FreeBSD__ || defined __riscos__
   requested.samples = pow( 2.0, floor( log2( sound_framesiz ) ) );
 #else			/* #ifdef __FreeBSD__ */
   requested.samples = sound_framesiz;
